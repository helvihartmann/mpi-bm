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

void Buffer::setloopvariables(size_t p, size_t innerRuntimeIterations_, int remoteRank_){
    packageCount = p / sizeof(int);
    innerRuntimeIterations = innerRuntimeIterations_;
    if (numberofcalls >= innerRuntimeIterations){
        numberofcalls = innerRuntimeIterations-1;
    }
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
            MPI_Rsend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            break;
        case 4:{
            int size = packageCount*MPI_BSEND_OVERHEAD;
            int *localbuffer = new int [size];
            MPI_Buffer_attach(localbuffer,size);
            MPI_Bsend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            MPI_Buffer_detach(localbuffer, &size);
        }
            break;
        case 5:{
            MPI_Request send_obj;
            MPI_Status status;
            //std::cout << queue_request.size() << std::endl;
            if (queue_request.size() >= numberofcalls){
                //std::cout << " clearing j: " << j << " numberofcalls: " << numberofcalls << std::endl;
                MPI_Wait (&queue_request.front(), &queue_status.front());
                queue_status.pop();
                queue_request.pop();
            }
            //std::cout << "sending j: " << j << std::endl;
            MPI_Isend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &send_obj);
            queue_status.push(status);
            queue_request.push (send_obj);
        }
            break;
        case 6:{
            MPI_Request send_obj;
            MPI_Status status;
            int *buffer_send;
            buffer_send = &buffer[(packageCount*j)%buffersize];
            if(j==0){
                //MPI_Request send_obj;
                MPI_Send_init (buffer_send, packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &send_obj);
            }
            MPI_Start (&send_obj);
            if (j%numberofcalls == 0 && j != (innerRuntimeIterations - 1)){
                //std::cout << "# Waiting...." << j << " " << packageCount << std::endl;
                MPI_Wait (&send_obj, &status);
            }
            else if(j == (innerRuntimeIterations - 1)){
                
                //buffer=&buffer[0];
                MPI_Request_free (&send_obj);
            }
        }
            break;
    }
}

void Buffer::recvBuffer(size_t j){
    switch (recvmode) {
        case 1:
        case 2:
        case 3:
        case 4:
            MPI_Recv((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            break;
        
        case 5:{
            MPI_Request recv_obj;
            MPI_Status status;
            if (queue_request.size() >= numberofcalls){
                MPI_Wait (&queue_request.front(), &queue_status.front());
                queue_status.pop();
                queue_request.pop();
            }
            
            MPI_Irecv((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &recv_obj);
            queue_status.push(status);
            queue_request.push(recv_obj);
        }break;
        case 6:{
            MPI_Request recv_obj;
            MPI_Status status;
            int *buffer_recv;
            buffer_recv = &buffer[(packageCount*j)%buffersize];
            if (j == 0){
                //MPI_Request recv_obj;
                MPI_Recv_init (buffer_recv, packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &recv_obj);
            }
            MPI_Start (&recv_obj);
            if (j%numberofcalls == 0 && j != (innerRuntimeIterations - 1)){
                MPI_Wait (&recv_obj, &status);
            }
            else if(j == (innerRuntimeIterations - 1)){
                MPI_Request_free (&recv_obj);
                buffer=&buffer[0];
            }
        }
            break;
    }
}

void Buffer::finalizeBuffer(){
    switch (recvmode) {
        case 1:break;
        case 2:break;
        case 3:break;
        case 4:break;
        case 5:{
            while (!queue_request.empty()){
                MPI_Wait(&queue_request.front(), &queue_status.front());
                queue_request.pop();
                queue_status.pop();
            }
        }break;
        case 6:break;
    }
}

void Buffer::checkBuffer(size_t *everythingcorrect_check){
    for (size_t i=0; i<packageCount; i++){
        if (buffer[i]!=rank){
            *everythingcorrect_check++;
        }
    }
}

