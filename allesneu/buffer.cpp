#include "buffer.h"

Buffer::Buffer(int size_, int rank_, unsigned int pipelinedepth_, int pipeline_, size_t buffersize_, std::vector<int>sender_vec_, std::vector<int>receiver_vec_, int numberofRootProcesses_, int numberofReceivers_) :
    size(size_),
    rank(rank_),
    pipelinedepth(pipelinedepth_),
    pipeline(pipeline_),
    buffersize(buffersize_),
    sender_vec(sender_vec_),
    receiver_vec(receiver_vec_),
    numberofRootProcesses(numberofRootProcesses_),
    numberofReceivers(numberofReceivers_)
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
    int remoterank;
    size_t index = 0;
    cycles_comm.assign(numberofReceivers,0);
    testwaitcounter.assign(numberofReceivers,0);
    
    switch (pipeline){
        case 0:{
            size_t i = 0;
            for(size_t j = 0; j < innerRuntimeIterations; j++){
                // wait for objects---------------------------
                while (queue_request.size() >= pipelinedepth*numberofReceivers){
                    waitstart.at(j) = timestamp.start();
                    MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
                    waitstop.at(j) = timestamp.stop();
                    testwaitcounter.front()+= timestamp.cycles();
                    queue_request.pop();
                }
                // fill queue---------------------------------
                for(int index_receiver = 0; index_receiver < numberofReceivers; index_receiver++){
                    remoterank = receiver_vec.at(index_receiver);
                    index = (packagecount*((j*numberofReceivers)+index_receiver))%(buffersize/sizeof(int));
                    timestamp.start();
                    MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &send_obj);
                    timestamp.stop();
                    queue_request.push(send_obj);
                    cycles_comm.at(index_receiver)+= timestamp.cycles();
                    i++;
                }
            }
            // empty queue---------------------------------
            while(!queue_request.empty()){
                MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
                queue_request.pop();
            }
        }
            break;
        case 1:{
            // iniate parameters--------------------------
            // vector holding all queues
            std::vector<queue<MPI_Request>> vec(numberofReceivers);
            // flags to keep track of status of objects
            int flag = 0;
            std::vector<size_t>sendcount (numberofReceivers,0);
            size_t sendcountsum = 0;
            
            // fill queue---------------------------------
             for(size_t j = 0; j < pipelinedepth; j++){
                 for(int index_receiver = 0; index_receiver < numberofReceivers; index_receiver++){
                     remoterank = receiver_vec.at(index_receiver);
                     index = packagecount*((j*numberofReceivers)+index_receiver)%(buffersize/sizeof(int));
                     timestamp.start();
                     MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &send_obj);
                     timestamp.stop();
                     cycles_comm.at(index_receiver)+= timestamp.cycles();
                     vec[index_receiver].push(send_obj);
                 }
             }
            // queue management ----------------------------
            
            while (sendcountsum != (numberofReceivers * innerRuntimeIterations)) {
                for(int index_receiver = 0; index_receiver < numberofReceivers; index_receiver++){
                    while (sendcount.at(index_receiver) != innerRuntimeIterations){ //loop over one receiver, while request objects say that sending is finished keeps sending new messages
                        MPI_Test(&vec[index_receiver].front(), &flag, MPI_STATUS_IGNORE);
                        testwaitcounter.at(index_receiver)++; //counts how often Wait was called for every receiver
                        if (flag == 1){
                            vec[index_receiver].pop();
                            timestamp.start();
                            index = packagecount*((sendcount.at(index_receiver)*numberofReceivers)+index_receiver)%(buffersize/sizeof(int));
                            remoterank = receiver_vec.at(index_receiver);
                            MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &send_obj);
                            sendcount.at(index_receiver)++;
                            sendcountsum++;
                            timestamp.stop();
                            vec[index_receiver].push(send_obj);
                            cycles_comm.at(index_receiver)+= timestamp.cycles();
                        }
                        else {
                            break;//breaks out of while loop
                        }
                    }
                }
            }
            
            // empy queue---------------------------------------
            for(int index_receiver = 0; index_receiver < numberofReceivers; index_receiver++){
                while(!vec[index_receiver].empty()){
                    MPI_Wait(&vec[index_receiver].front(), MPI_STATUS_IGNORE);
                    vec[index_receiver].pop();
                }
            }
        }
        break;
    }
}

void Buffer::receivebuffer(){
    std::queue<MPI_Request> queue_request;
    MPI_Request recv_obj;
    int remoterank;
    size_t index = 0;
    cycles_comm.assign(numberofRootProcesses,0);
    testwaitcounter.assign(numberofRootProcesses,0);
    switch (pipeline){
        case 0:{
            size_t i = 0;
            for(size_t j = 0; j < innerRuntimeIterations; j++){
                // wait for objects in queue-------------------------------------
                while(queue_request.size() >= pipelinedepth*numberofReceivers){
                    timestamp.start();
                    MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
                    timestamp.stop();
                    testwaitcounter.front()+=timestamp.cycles();
                    queue_request.pop();
                }
                // fill queue-------------------------------------
                for(int index_sender = 0; index_sender < numberofRootProcesses; index_sender++){
                    //std::cout << index_sender << "/" << numberofRootProcesses << std::endl;
                    //std::cout << rank << ", " << sender_vec.at(index_sender) << std::endl;
                    remoterank = sender_vec.at(index_sender);
                    index = (packagecount * ((j * numberofRootProcesses) + index_sender))%(buffersize/sizeof(int));
                    timestamp.start();
                    MPI_Irecv((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &recv_obj);
                    timestamp.stop();
                    cycles_comm.at(index_sender)+= timestamp.cycles();
                    queue_request.push(recv_obj);
                    i++;
                }
            }
            // empty queue-------------------------------------
            while(!queue_request.empty()){
                MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
                queue_request.pop();
            }
        }
            break;
        case 1:{
            // iniate parameters--------------------------
            // vector holding all queues
            std::vector<queue<MPI_Request>> vec(numberofRootProcesses);
            // flags
            int flag = 0;
            size_t index;
            std::vector<size_t>recvcount (numberofRootProcesses,0);
            size_t recvcountsum = 0;
            // fill queue-----------------------------------
            for(size_t j = 0; j < pipelinedepth; j++){
                for(int index_sender = 0; index_sender < numberofRootProcesses; index_sender++){
                    remoterank = sender_vec.at(index_sender);
                    index = packagecount*((j*numberofRootProcesses)+index_sender)%(buffersize/sizeof(int));
                    timestamp.start();
                    MPI_Irecv((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &recv_obj);
                    timestamp.stop();
                    cycles_comm.at(index_sender)+= timestamp.cycles();
                    vec[index_sender].push(recv_obj);
                }
            }
            // queue management-----------------------------
            while (recvcountsum != (numberofRootProcesses * innerRuntimeIterations)) {
                for(int index_sender = 0; index_sender < numberofRootProcesses; index_sender++){
                    while (recvcount.at(index_sender) != innerRuntimeIterations){ //loop over one receiver, while request objects say that sending is finished keeps sending new messages
                        MPI_Test(&vec[index_sender].front(), &flag, MPI_STATUS_IGNORE);
                        testwaitcounter.at(index_sender)++; //counts how often Wait was called for every receiver
                        if (flag == 1){
                            vec[index_sender].pop();
                            timestamp.start();
                            index = packagecount*((recvcount.at(index_sender)*numberofRootProcesses)+index_sender)%(buffersize/sizeof(int));
                            remoterank = sender_vec.at(index_sender);
                            MPI_Irecv((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &recv_obj);
                            recvcount.at(index_sender)++;
                            recvcountsum++;
                            timestamp.stop();
                            vec[index_sender].push(recv_obj);
                            cycles_comm.at(index_sender)+= timestamp.cycles();
                        }
                        else {
                            break;//breaks out of while loop
                        }
                    }
                }
            }
            
            /*for(size_t j = pipelinedepth; j < innerRuntimeIterations; j++){
                testflag = 0;
                testflagvector.assign(numberofRootProcesses,0);
                while (testflag < numberofRootProcesses){
                    testflag = 0;
                    for(int index_sender = 0; index_sender < numberofRootProcesses; index_sender++){
                        if (testflagvector.at(index_sender) == 0){
                            remoterank = sender_vec.at(index_sender);
                            index = packagecount*((j*numberofRootProcesses)+index_sender)%(buffersize/sizeof(int));
                            MPI_Test(&vec[index_sender].front(), &flag, MPI_STATUS_IGNORE);
                            testwaitcounter.at(index_sender)++;
                            if (flag == 1){
                                vec[index_sender].pop();
                                timestamp.start();
                                MPI_Irecv((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &recv_obj);
                                timestamp.stop();
                                cycles_comm.at(index_sender)+= timestamp.cycles();
                                vec[index_sender].push(recv_obj);
                                testflagvector.at(index_sender)=1;
                            }
                        }
                        testflag = testflag + testflagvector.at(index_sender);
                    }
                }
            }*/
            // empty queue-------------------------------------
            for(int index_sender = 0; index_sender < numberofRootProcesses; index_sender++){
                while(!vec[index_sender].empty()){
                    MPI_Wait(&vec[index_sender].front(), MPI_STATUS_IGNORE);
                    vec[index_sender].pop();
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
    size_t i = 0;
    ostringstream oss;
    oss << (packagecount*sizeof(int)) << "_x" << numberofRootProcesses << "_p" << pipelinedepth << "_n" << size << "_" << rank << ".hist";
    ofstream myfile;
    myfile.open(oss.str());
    //myfile << "# average time = " << (time/innerRuntimeIterations)*1000000 << " microseconds for " << innerRuntimeIterations << " iterations\n";
    myfile << "# P(s)IS P(e)Is TIS P(s)Ws P(e)Ws TWs P(s)RS P(e)Rs TIS P(s)Wr P(e)Wr TWr" << "\n";
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        myfile << sendstart.at(j) << " " << sendstop.at(j) << " " << ((sendstop.at(j)-sendstart.at(j))/2000) << " " << waitstart.at(j) << " " << waitstop.at(j) << " " << ((waitstop.at(j)-waitstart.at(j))/2000) << " " << recvstart.at(j) << " " << recvstop.at(j) << " " << ((recvstop.at(j)-recvstart.at(j))/2000) << " " << waitrstart.at(j) << " " << waitrstop.at(j) << " " << ((waitrstop.at(j)-waitrstart.at(j))/2000) << " " << i << "\n";
        i++;
    }
    
    myfile.close();
}