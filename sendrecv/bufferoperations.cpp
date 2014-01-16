#include "bufferoperations.h"

/*  */

    
Bufferoperations::Bufferoperations(const int* sendmode_, Mpi *mpi1pter,int rank)
    : sendmode(sendmode_)
{
    mpi1 = *mpi1pter;
    buffer = new int [buffersize];//von VoLi angewiesene Größe
    std::cout<<"# "<<buffersize*sizeof(int)<<"B allocated \n";
    for(size_t i=0; i<buffersize; i++){
        buffer[i]=rank;
        
    }
}

void Bufferoperations::setloopvariables(size_t p, size_t innerRuntimeIterations_){
    packagesize_temp = p;
    innerRuntimeIterations = innerRuntimeIterations_;
}

void Bufferoperations::initalizeBuffer(int rank){
    
}

/*
 buffer = new int[16]
 
 send(buffer, 20) -> ganzer buffer
 send(buffer, 4) -> nur die ersten 4 ints
 send(buffer + 4, 4) -> die zweiten 4 ints
 for (i = 0; i < 5; ++i)
   send(buffer + i * 4, 4)
 
 */

/*void Bufferoperations::specifyBuffer(){
    mpi1.performBufferspecification(buffer,packagesize_temp);
}*/

void Bufferoperations::sendBuffer(){
    for(size_t j=0; j<innerRuntimeIterations; j++){
        /*int* buffertmp;
        buffertmp = buffer+ (j*packagesize_temp);
        std::cout<<"buffertmp "<<*buffertmp<<"\n";
        mpi1.performsend(buffertmp,packagesize_temp,MPI_INT,1,j,MPI_COMM_WORLD, sendmode);*/
        mpi1.performsend((buffer + (packagesize_temp*j)),packagesize_temp,MPI_INT,1,j,MPI_COMM_WORLD, sendmode);

    }
}

void Bufferoperations::recvBuffer(){
    for(size_t j=0; j<innerRuntimeIterations; j++){
        /*int* buffertmp;
        buffertmp = buffer+ (j*packagesize_temp);
        mpi1.performrecv(buffertmp,packagesize_temp,MPI_INT,0,j,
                         MPI_COMM_WORLD,MPI_STATUS_IGNORE);*/
        mpi1.performrecv((buffer + (packagesize_temp*j)),packagesize_temp,MPI_INT,0,j,
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
