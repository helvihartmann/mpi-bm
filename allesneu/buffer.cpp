#include "buffer.h"

Buffer::Buffer(int size_, int rank_, unsigned int pipelinedepth_, int pipeline_, int numberofRootProcesses_, size_t buffersize_) :
    size(size_),
    rank(rank_),
    pipelinedepth(pipelinedepth_),
    pipeline(pipeline_),
    numberofRootProcesses(numberofRootProcesses_),
    buffersize(buffersize_)
{
    std::cout << "# allocating buffer..." << std::endl;
    
    buffer = new int [buffersize / sizeof(int)];
    
    std::cout << "# " << buffersize << " B allocated, initializing...\n" << std::endl;
    
    for (size_t i=0; i < (buffersize / sizeof(int)); i++){
        buffer[i]=rank;
    }
    
    std::cout << "# buffer initialized.\n" << std::endl;
}

void Buffer::setloopvariables(size_t packagecount_, size_t innerRuntimeIterations_){
    innerRuntimeIterations = innerRuntimeIterations_;
    packagecount = packagecount_;
    sendstart.resize(innerRuntimeIterations);
    sendstop.resize(innerRuntimeIterations);
    waitstart.resize(innerRuntimeIterations);
    waitstop.resize(innerRuntimeIterations);
    recvstart.resize(innerRuntimeIterations);
    recvstop.resize(innerRuntimeIterations);
    waitrstart.resize(innerRuntimeIterations);
    waitrstop.resize(innerRuntimeIterations);
}

void Buffer::sendbuffer(){
    std::queue<MPI_Request> queue_request;
    MPI_Request send_obj;
    switch (pipeline){
        case 0:{
            size_t i = 0;
            for(size_t j = 0; j < innerRuntimeIterations; j++){
                while (queue_request.size() >= pipelinedepth*(size-numberofRootProcesses)){
                    waitstart.at(j) = timestamp.start();
                    MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
                    waitstop.at(j) = timestamp.stop();
                    queue_request.pop();
                }
                for(int remoterank = 1; remoterank < size; remoterank++){
                    if ((remoterank%2) == 0 && remoterank < (numberofRootProcesses*2)){
                    }
                    else{
                        //std::cout << "remoterank: " << remoterank << " sender: " << rank << std::endl;
                        
                        
                        sendstart.at(j) = timestamp.start();
                        MPI_Issend((buffer + (((packagecount*j)+i)%(buffersize/sizeof(int)))), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &send_obj);
                        sendstop.at(j) = timestamp.stop();
                        queue_request.push(send_obj);
                        i++;
                    }
                    
                }
            }
            while(!queue_request.empty()){
                MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
                queue_request.pop();
            }
        }
        break;
        case 1:{
            std::vector<queue<MPI_Request>> vec;
            int flag = 0;
            size_t index;
            for (int remoterank = 0; remoterank < size; remoterank++){
                vec.push_back(queue<MPI_Request>());
            }
             for(size_t j = 0; j < innerRuntimeIterations; j++){
                 size_t i = 0;
                 for(int remoterank = numberofRootProcesses; remoterank < size; remoterank++){
                     index = packagecount*((j*size)+rank)%(buffersize/sizeof(int));
                     std::cout << "sender: " << index << std::endl;
                     if (vec[remoterank].size() <= pipelinedepth){
                         MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &send_obj);
                         vec[remoterank].push(send_obj);
                     }
                     else{
                         MPI_Test(&vec[remoterank].front(), &flag, MPI_STATUS_IGNORE);
                         
                         if (flag == 1){
                             vec[remoterank].pop();
                             MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &send_obj);
                             vec[remoterank].push(send_obj);
                         }
                     }
                 }
             }
            for(int remoterank = numberofRootProcesses; remoterank < size; remoterank++){
                while(!vec[remoterank].empty()){
                    MPI_Wait(&vec[remoterank].front(), MPI_STATUS_IGNORE);
                    vec[remoterank].pop();
                }
            }
        }
        break;
    }
}

void Buffer::receivebuffer(){
    std::queue<MPI_Request> queue_request;
    MPI_Request recv_obj;
    switch (pipeline){
        case 0:{
            size_t i = 0;
            for(size_t j = 0; j < innerRuntimeIterations; j++){
                while(queue_request.size() >= pipelinedepth*(size-numberofRootProcesses)){
                    waitrstart.at(j) = timestamp.start();
                    MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
                    waitrstop.at(j) = timestamp.stop();
                    queue_request.pop();
                }
                for(int remoterank = 0; remoterank < (numberofRootProcesses*2); remoterank = remoterank + 2){
                    recvstart.at(j) = timestamp.start();
                    MPI_Irecv((buffer + (((packagecount*j)+i)%(buffersize/sizeof(int)))), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &recv_obj);
                        queue_request.push(recv_obj);
                    recvstop.at(j) = timestamp.stop();
                    
                }
                i++;
            }
            while(!queue_request.empty()){
                MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
                queue_request.pop();
            }
        }
            break;
        case 1:{
            std::vector<queue<MPI_Request>> vec;
            int flag = 0;
            size_t index;
            for(int remoterank = 0; remoterank < numberofRootProcesses; remoterank++){
                vec.push_back(queue<MPI_Request>());
            }
            for(size_t j = 0; j < innerRuntimeIterations; j++){
                for(int remoterank = 0; remoterank < numberofRootProcesses; remoterank++){index = packagecount*((j*size)+remoterank)%(buffersize/sizeof(int));
                    if (vec[remoterank].size() <= pipelinedepth){
                        MPI_Irecv((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &recv_obj);
                        vec[remoterank].push(recv_obj);

                    }
                    else{
                        std::cout << " test for receive, innerruntimeiter " << j << " of " << innerRuntimeIterations << ", rank " << rank << ", packagecount " << packagecount<< std::endl;
                        MPI_Test(&vec[remoterank].front(), &flag, MPI_STATUS_IGNORE);
                        std::cout << " tested " <<  flag << std::endl;
                        if (flag == 1){
                            vec[remoterank].pop();
                            MPI_Irecv((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &recv_obj);
                            vec[remoterank].push(recv_obj);
                        }
                    }
                }
            }
            for(int remoterank = 0; remoterank < numberofRootProcesses; remoterank++){
                while(!vec[remoterank].empty()){
                    MPI_Wait(&vec[remoterank].front(), MPI_STATUS_IGNORE);
                    vec[remoterank].pop();
                }
            }
        }
            break;
    }
}

/*void Buffer::everythingcorrectcheck(){
    for (size_t i=0; i < (packagecount*inn); i++){
        buffer[i]=rank;
    }
}*/

void Buffer::printsingletime(){
    size_t i;
    ostringstream oss;
    oss << (packagecount*sizeof(int)) << "_x" << numberofRootProcesses << "_p" << pipelinedepth << "_n" << size << "_" << rank << ".hist";
    
    
    ofstream myfile;
    myfile.open(oss.str());
    //myfile << "# average time = " << (time/innerRuntimeIterations)*1000000 << " microseconds for " << innerRuntimeIterations << " iterations\n";
    myfile << "# P(s)IS P(e)Is TIS P(s)Ws P(e)Ws TWs P(s)RS P(e)Rs TIS P(s)Wr P(e)Wr TWr" << "\n";
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        myfile << sendstart.at(j) << " " << sendstop.at(j) << " " << ((sendstop.at(j)-sendstart.at(j))/2000) << " " << waitstart.at(j) << " " << waitstop.at(j) << " " << ((waitstop.at(j)-waitstart.at(j))/2000) << " " << recvstart.at(j) << " " << recvstop.at(j) << " " << ((recvstop.at(j)-recvstart.at(j))/2000) << " " << waitrstart.at(j) << " " << waitrstop.at(j) << " " << ((waitrstop.at(j)-waitrstart.at(j))/2000) << i << "\n";
        i++;
    }
    
    myfile.close();
}