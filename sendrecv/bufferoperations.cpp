#include "bufferoperations.h"

/*  */

    
Bufferoperations::Bufferoperations(size_t p, size_t innerRuntimeIterations_, const int* sendmode_, Mpi *mpi1pter)
    : sendmode(sendmode_)
{
    mpi1 = *mpi1pter;
    packagesize_temp = p;
    innerRuntimeIterations = innerRuntimeIterations_;
}


void Bufferoperations::allocateBuffer(){
    buffer= new int [packagesize_temp];
}

void Bufferoperations::initalizeBuffer(){
    for(size_t i=0; i<packagesize_temp; i++){
        buffer[i]=1;

    }
}

/*void Bufferoperations::specifyBuffer(){
    mpi1.performBufferspecification(buffer,packagesize_temp);
}*/

void Bufferoperations::sendBuffer(){
    for(int j=0; j<innerRuntimeIterations; j++){
        mpi1.performsend(buffer,packagesize_temp,MPI_INT,1,j,MPI_COMM_WORLD, sendmode);
    }
}

void Bufferoperations::recvBuffer(){
    for(int j=0; j<innerRuntimeIterations; j++){
        mpi1.performrecv(buffer,packagesize_temp,MPI_INT,0,j,
                         MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
}

void Bufferoperations::checkBuffer(size_t *everythingcorrect_check){
    for(size_t i=0;i<packagesize_temp;i++){
        if(buffer[i]!=1){
            everythingcorrect_check++;
        }
    }
}

/*void Bufferoperations::detachBuffer(){
    mpi1.performBufferdetach(buffer,packagesize_temp);
}*/

void Bufferoperations::freeBuffer(){
    delete [] buffer;
}
