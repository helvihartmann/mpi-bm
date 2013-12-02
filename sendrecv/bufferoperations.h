#ifndef BUFFEROPERATIONS_H
#define BUFFEROPERATIONS_H

#include "classmpi3.h"
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 14.11.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes even more than 1GB
 This version iterates over the packege size to assure for every run the same conditions on the fles
 and prints everything into 503.out*/

class Bufferoperations{
private:
    int *scounts,*rcounts;
    size_t packagesize_temp;
    size_t timepackageissend;
    Mpi mpi1;
public:
    
    void setPackagesizeTmp(size_t);
    
    void setIterations2(size_t);
    
    void allocateBuffer();
    
    void initalizeBuffer();
    
    void sendBuffer(int const);
    
    void recvBuffer();
    
    void checkBuffer(size_t*);
    
    void freeBuffer();
    
};

#endif /*BUFFEROPERATIONS_H*/