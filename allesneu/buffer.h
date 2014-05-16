#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <queue>
#include <mpi.h>
/**/

class Buffer{
private:
    int size;
    int rank;
    
    int pipelinedepth;
    int numberofRootProcesses;
    size_t buffersize;

    int *buffer;
public:
    
    Buffer(int size, int rank, unsigned int pipelinedepth, int numberofrootprocesses, size_t buffersize);
    
    void sendbuffer(size_t packagecount, size_t innerRuntimeIterations);
    
    void receivebuffer(size_t packagecount, size_t innerRuntimeIterations);
};

#endif /*BUFFER_H*/