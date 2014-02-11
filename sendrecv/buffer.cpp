#include "buffer.h"

/*  */

    
Buffer::Buffer(int sendmode_, int rank_, size_t buffersize_) :
    buffersize(buffersize_),
    sendmode(sendmode_),
    rank(rank_)
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


void Buffer::sendBuffer(size_t j){
    
    switch (sendmode) {
        case 1:
            MPI_Send((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            break;
        case 2:
            MPI_Ssend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            break;
        case 3:
            MPI_Bsend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            break;
        case 4:
            MPI_Request send_obj;
            MPI_Status status;
            int *buffer_send;
            buffer_send = &buffer[(packageCount*j)%buffersize];
            if(j==0){
                MPI_Request send_obj;
                MPI_Send_init (buffer_send, packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &send_obj);
            }
            MPI_Start (&send_obj);
            MPI_Wait (&send_obj, &status);
            if(j == (innerRuntimeIterations - 1)){
                //buffer=&buffer[0];
                MPI_Request_free (&send_obj);
            }
            break;
        default:
                MPI_Send((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            break;
    }
    
}

void Buffer::recvBuffer(size_t j){
    switch (sendmode) {
        case 1:
        case 2:
        case 3:
            MPI_Recv((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            break;
        case 4:
            MPI_Request recv_obj;
            MPI_Status status;
            int *buffer_recv;
            buffer_recv = &buffer[(packageCount*j)%buffersize];
            if(j == 0){
                MPI_Request recv_obj;
                MPI_Recv_init (buffer_recv, packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &recv_obj);
            }
            MPI_Start (&recv_obj);
            MPI_Wait (&recv_obj, &status);
            if(j == (innerRuntimeIterations - 1)){
                MPI_Request_free (&recv_obj);
                buffer=&buffer[0];
            }
            break;
    }
}

void Buffer::checkBuffer(size_t *everythingcorrect_check){
    for (size_t i=0; i<packageCount; i++){
        if (buffer[i]!=rank){
            *everythingcorrect_check++;
        }
    }
}

