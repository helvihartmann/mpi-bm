#include "buffer.h"

Buffer::Buffer(int size_, int rank_, unsigned int pipelinedepth_, int numberofRootProcesses_, size_t buffersize_) :
    size(size_),
    rank(rank_),
    pipelinedepth(pipelinedepth_),
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
    singletime.resize(innerRuntimeIterations);
}

void Buffer::sendbuffer(){
    std::queue<MPI_Request> queue_request;
    MPI_Request send_obj;
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        while (queue_request.size() >= pipelinedepth){
            MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
            queue_request.pop();
        }
        for(int remoterank = numberofRootProcesses; remoterank < size; remoterank++){
            timestamp.stop();
            singletime.at(j)=timestamp.cycles();
            timestamp.start();
            MPI_Issend((buffer + ((packagecount*j)%(buffersize/sizeof(int)))), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &send_obj);
            queue_request.push(send_obj);
        }
    }
    while(!queue_request.empty()){
        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
        queue_request.pop();
    }
}

void Buffer::receivebuffer(){
    std::queue<MPI_Request> queue_request;
    MPI_Request recv_obj;
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        while(queue_request.size() >= pipelinedepth){
            MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
            queue_request.pop();
        }
        for(int remoterank = 0; remoterank < numberofRootProcesses; remoterank++){
            MPI_Irecv((buffer + ((packagecount*j)%(buffersize/sizeof(int)))), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &recv_obj);
            queue_request.push(recv_obj);
        }
    }
    while(!queue_request.empty()){
        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
        queue_request.pop();
    }
}

void Buffer::printsingletime(double time, int statisticaliteration){
    size_t i;
    ostringstream oss;
    oss << (packagecount*sizeof(int)) << "_p" << pipelinedepth << "_i" << innerRuntimeIterations << "_o" << statisticaliteration << ".hist";
    
    
    ofstream myfile;
    myfile.open(oss.str());
    myfile << "# average time = " << (time/innerRuntimeIterations)*1000000 << " microseconds for " << innerRuntimeIterations << " iterations\n";
    myfile << "# processor cycles time[ms]" << "\n";
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        myfile << singletime.at(j) << " " << (singletime.at(j)/2000) << " " << i << "\n";
        i++;
    }
    
    myfile.close();
}