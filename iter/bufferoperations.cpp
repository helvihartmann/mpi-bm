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
    counts= new int [packagesize_temp];
    //rcounts= new int [packagesize_temp];
}

void Bufferoperations::initalizeBuffer(){
    for(size_t i=0; i<packagesize_temp; i++){
        counts[i]=1;

    }
}

void Bufferoperations::sendBuffer(){
    for(int j=0; j<innerRuntimeIterations; j++){
        //std::cout << "scounts  "<< *scounts<<"\n";
        mpi1.performsend(counts,packagesize_temp,MPI_INT,1,j,MPI_COMM_WORLD, sendmode);
    }
}

void Bufferoperations::recvBuffer(){
    for(int j=0; j<innerRuntimeIterations; j++){
        mpi1.performrecv(counts,packagesize_temp,MPI_INT,0,j,
                         MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    //std::cout << "rcounts  "<< *rcounts<<"\n";
}

void Bufferoperations::checkBuffer(size_t *everythingcorrect_check){
    //std::cout << "rcounts oben "<< *rcounts<<"\n \n \n";
    for(size_t i=0;i<packagesize_temp;i++){
        if(counts[i]!=1){
            everythingcorrect_check++;
        }
    }
}

void Bufferoperations::freeBuffer(){
    delete [] counts;
    //delete [] rcounts;
}
