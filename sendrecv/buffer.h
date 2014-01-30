#ifndef BUFFER_H
#define BUFFER_H

#include "classmpi3.h"
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 14.11.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes even more than 1GB
 This version iterates over the packege size to assure for every run the same conditions on the fles
 and prints everything into 503.out*/

class Buffer{
private:
    const size_t buffersize = 50000000000;

    const int sendmode;
    Mpi &mpi;
    int *buffer;
    
    size_t packageCount;
    size_t innerRuntimeIterations;
    int remoteRank;
public:
    
    Buffer(int, Mpi&, int);
    ~Buffer();

    void setloopvariables(size_t, size_t,int);
    
    void sendBuffer();
    
    void recvBuffer();
    
    void checkBuffer(size_t *);
};

#endif /*BUFFER_H*/