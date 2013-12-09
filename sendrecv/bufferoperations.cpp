#include "mpi.h"
#include "bufferoperations.h"

/*  */

    
Bufferoperations::Bufferoperations(size_t p, size_t iterations2, int sendmode){
    packagesize_temp = p;
    timepackageissend = iterations2;
    sendmode = sendmode;
}


void Bufferoperations::allocateBuffer(){
    scounts= new int [packagesize_temp];
    rcounts= new int [packagesize_temp];
}

void Bufferoperations::initalizeBuffer(){
    for(size_t i=0; i<packagesize_temp; i++){
        scounts[i]=1;

    }
}

void Bufferoperations::sendBuffer(){
    for(int j=0; j<timepackageissend; j++){
        //std::cout << "scounts  "<< *scounts<<"\n";
        mpi1.performsend(scounts,packagesize_temp,MPI_INT,1,j,MPI_COMM_WORLD);
    }
}

void Bufferoperations::recvBuffer(){
    for(int j=0; j<timepackageissend; j++){
        mpi1.performrecv(rcounts,packagesize_temp,MPI_INT,0,j,
                         MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    //std::cout << "rcounts  "<< *rcounts<<"\n";
}

void Bufferoperations::checkBuffer(size_t *everythingcorrect_check){
    //std::cout << "rcounts oben "<< *rcounts<<"\n \n \n";
    for(size_t i=0;i<packagesize_temp;i++){
        if(rcounts[i]!=1){
            everythingcorrect_check++;
        }
    }
}

void Bufferoperations::freeBuffer(){
    delete [] scounts;
    delete [] rcounts;
}
