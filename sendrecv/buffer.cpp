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

void Buffer::setloopvariables(size_t p, size_t innerRuntimeIterations_){
    packageCount = p / sizeof(int);
    innerRuntimeIterations = innerRuntimeIterations_;
    if (numberofcalls >= innerRuntimeIterations){
        numberofcalls = innerRuntimeIterations-1;
    }
}


void Buffer::sendBuffer(unsigned int remoteRank){
    switch (sendmode) {
        case 1:{
            //int size = packageCount*MPI_BSEND_OVERHEAD;
            //int *localbuffer = new int [size];
            //MPI_Buffer_attach(localbuffer,size);
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Send((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            }
            //MPI_Buffer_detach(localbuffer, &size);
        }break;
        case 2:{
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Ssend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            }
        }break;
        case 3:{
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Rsend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            }
        }break;
        case 4:{
            for(size_t j=0; j<innerRuntimeIterations; j++){
                int size = packageCount*MPI_BSEND_OVERHEAD;
            int *localbuffer = new int [size];
            MPI_Buffer_attach(localbuffer,size);
            MPI_Bsend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD);
            MPI_Buffer_detach(localbuffer, &size);
            }
        }break;
        case 5:{
            std::queue<MPI_Request> queue_request;
            std::queue<MPI_Status> queue_status;
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Request send_obj;
                MPI_Status status;
                if (queue_request.size() >= numberofcalls){
                    MPI_Wait (&queue_request.front(), &queue_status.front());
                    queue_status.pop();
                    queue_request.pop();
                }
                MPI_Issend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &send_obj);
                queue_status.push(status);
                queue_request.push (send_obj);
            }
            while (!queue_request.empty()){
                MPI_Wait(&queue_request.front(), &queue_status.front());
                queue_request.pop();
                queue_status.pop();
            }
        }break;
        case 6:{
            MPI_Status status;
            MPI_Request request;
            int *buffer_tmp;
            buffer_tmp = &buffer[0];
            MPI_Send_init (buffer_tmp, packageCount, MPI_INT, remoteRank, 1, MPI_COMM_WORLD, &request);
            for(size_t j=0; j<innerRuntimeIterations; j++){
                //std::cout << "(packageCount*j)%buffersize: " << (packageCount*j)%buffersize << std::endl;
                MPI_Start (&request);
                MPI_Wait(&request,&status);
                buffer_tmp = &buffer[(packageCount*j)%buffersize];
            }
            MPI_Request_free(&request);
        }break;
    }
}

void Buffer::recvBuffer(unsigned int remoteRank){
    switch (recvmode) {
        case 1:
        case 2:
        case 3:
        case 4:
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Recv((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
                        break;
        
        case 5:{
            std::queue<MPI_Request> queue_request;
            std::queue<MPI_Status> queue_status;
            for(size_t j=0; j<innerRuntimeIterations; j++){
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
            }
            while (!queue_request.empty()){
                MPI_Wait(&queue_request.front(), &queue_status.front());
                queue_request.pop();
                queue_status.pop();
            }
        }break;
        case 6:{
            MPI_Status status;
            MPI_Request request;
            int *buffer_tmp;
            buffer_tmp = &buffer[0];
            MPI_Recv_init (buffer_tmp, packageCount, MPI_INT, remoteRank, 1,MPI_COMM_WORLD, &request);
            for(size_t j=0; j<innerRuntimeIterations; j++){
                MPI_Start (&request);
                MPI_Wait (&request, &status);
                buffer_tmp = &buffer[(packageCount*j)%buffersize];
            }
            MPI_Request_free(&request);
        }break;
    }
}


void Buffer::checkBuffer(size_t *everythingcorrect_check){
    for (size_t i=0; i<packageCount; i++){
        if (buffer[i]!=rank){
            *everythingcorrect_check++;
        }
    }
}

