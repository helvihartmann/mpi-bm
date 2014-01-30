#include "buffer.h"

/*  */

    
Buffer::Buffer(int sendmode_, int rank, size_t buffersize_) :
    buffersize(buffersize_),
    sendmode(sendmode_)
{
    std::cout << "# allocating buffer..." << std::endl;

    buffer = new int [buffersize / sizeof(int)];//von VoLi angewiesene Größe

    std::cout << "# " << buffersize << " B allocated, initializing...\n" << std::endl;

    for (size_t i=0; i<buffersize / sizeof(int); i++){
        buffer[i]=rank;
    }

    std::cout << "# buffer initialized.\n" << std::endl;
}

Buffer::~Buffer(){
    delete [] buffer;
}

void Buffer::setloopvariables(size_t p, size_t innerRuntimeIterations_, int remoteRank_){
    packageCount = p / sizeof(int);
    innerRuntimeIterations = innerRuntimeIterations_;
    remoteRank = remoteRank_;
}

void Buffer::sendBuffer(){
    
    switch (sendmode) {
        case 1:
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Send((buffer + (packageCount*j)), packageCount,MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            }
            break;
                case 2:
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Ssend((buffer + (packageCount*j)), packageCount,MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            }
            break;
        case 3:
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Bsend((buffer + (packageCount*j)), packageCount,MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            }

            break;
            
        default:
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Send((buffer + (packageCount*j)), packageCount,MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            }
            break;
    }

}

void Buffer::recvBuffer(){
    for(size_t j=0; j<innerRuntimeIterations; j++){
        MPI_Recv((buffer + (packageCount*j)), packageCount,MPI_INT,remoteRank, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void Buffer::checkBuffer(size_t *everythingcorrect_check){
    for (size_t i=0; i<packageCount; i++){
        if (buffer[i]!=1){
            everythingcorrect_check++;
        }
    }
}

