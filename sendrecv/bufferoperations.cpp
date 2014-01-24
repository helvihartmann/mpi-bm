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

void Bufferoperations::setloopvariables(size_t p, size_t innerRuntimeIterations_, int tag_){
    packagesize_temp = p;
    innerRuntimeIterations = innerRuntimeIterations_;
    tag = tag_;
}


/*void Bufferoperations::specifyBuffer(){
    mpi1.performBufferspecification(buffer,packagesize_temp);
}*/

void Bufferoperations::sendBuffer(){
    for(size_t j=0; j<innerRuntimeIterations; j++){
        mpi1.performsend((buffer + (packagesize_temp*j)),packagesize_temp,MPI_INT,tag,j,MPI_COMM_WORLD, sendmode);

    }
}

void Bufferoperations::recvBuffer(){
    for(size_t j=0; j<innerRuntimeIterations; j++){
        mpi1.performrecv((buffer + (packagesize_temp*j)),packagesize_temp,MPI_INT,tag,j,
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
