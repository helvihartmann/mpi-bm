#ifndef BUFFER_H
#define BUFFER_H

#include <mpi.h>
#include <iostream>
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 14.11.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes even more than 1GB
 This version iterates over the packege size to assure for every run the same conditions on the fles
 and prints everything into 503.out*/

class Buffer{
private:
    size_t buffersize;

    const int sendmode;
    const int recvmode;
    int rank;
    int *buffer;
    
    size_t packageCount;
    size_t innerRuntimeIterations;
    int remoteRank;
public:
    
    Buffer(int, int, int, size_t);
    ~Buffer();

    void setloopvariables(size_t, size_t,int);
    
    void sendBuffer(size_t);
    
    void recvBuffer(size_t);
    
    void checkBuffer(size_t *);
};

#endif /*BUFFER_H*/