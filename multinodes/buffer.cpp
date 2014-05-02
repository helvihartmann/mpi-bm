#include "buffer.h"


/*  */

    
Buffer::Buffer(int sendmode_, int recvmode_, size_t numberofcalls_, int rank_, size_t buffersize_) :
    buffersize(buffersize_),
    sendmode(sendmode_),
    recvmode(recvmode_),
    numberofcalls(numberofcalls_),
    rank(rank_)
{
    std::cout << "# allocating buffer..." << std::endl;
    
    buffer = new int [buffersize / sizeof(int)];

    std::cout << "# " << buffersize << " B allocated, initializing...\n" << std::endl;

    for (size_t i=0; i<buffersize / sizeof(int); i++){
        buffer[i]=rank;
    }

    std::cout << "# buffer initialized.\n" << std::endl;
    
    
}

Buffer::~Buffer(){
    delete [] buffer;
}

void Buffer::setloopvariables(size_t packageCount_, size_t innerRuntimeIterations_){
    packageCount = packageCount_;
    innerRuntimeIterations = innerRuntimeIterations_;
}


void Buffer::sendBuffer(unsigned int numberofRootProcesses, int size){
    std::queue<MPI_Request> queue_request;
    std::queue<MPI_Status> queue_status;
    MPI_Request send_obj;
    MPI_Status status;
    size_t rein = 0;
    size_t raus = 0;
    for(size_t j=0; j<innerRuntimeIterations; j++){
        while (queue_request.size() >= numberofcalls){
            MPI_Wait (&queue_request.front(), &queue_status.front());
            queue_status.pop();
            queue_request.pop();
            raus++;
        }
        for (int remoteRank = numberofRootProcesses; remoteRank < size; remoteRank++) {
            MPI_Issend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &send_obj);
            queue_status.push(status);
            queue_request.push (send_obj);
            rein++;
        }
    }
    std::cout << "rein " << rein << std::endl;
    while (!queue_request.empty()){
        MPI_Wait(&queue_request.front(), &queue_status.front());
        queue_request.pop();
        queue_status.pop();
        raus++;
        
    }
    std::cout << "raus " << raus << std::endl;
}

void Buffer::recvBuffer(unsigned int numberofRootProcesses,int size){
    MPI_Status status;
    MPI_Request recv_obj;
    size_t received = 0;
    for(size_t j=0; j<innerRuntimeIterations; j++){
        for (int remoteRank = 0; remoteRank < numberofRootProcesses; remoteRank++) {
            MPI_Irecv((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &recv_obj);
            MPI_Wait(&recv_obj,&status);
            received++;
        }
    }
    std::cout << "received " << received << std::endl;
}



