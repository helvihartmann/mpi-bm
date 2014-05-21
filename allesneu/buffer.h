#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <queue>
#include <mpi.h>
#include "tsc.h"
#include <fstream>
#include <sstream>

using namespace std;
/**/

class Buffer{
private:
    int size;
    int rank;
    
    int pipelinedepth;
    int numberofRootProcesses;
    size_t buffersize;

    int *buffer;
    
    size_t innerRuntimeIterations;
    size_t packagecount;
    
    std::vector<unsigned long long>singletime;
    
    TimeStampCounter timestamp;
public:
    
    Buffer(int size, int rank, unsigned int pipelinedepth, int numberofrootprocesses, size_t buffersize);
    
    void setloopvariables(size_t packagecount, size_t innerRuntimeIterations);
    void sendbuffer();
    
    void receivebuffer();
    
    void printsingletime(double time);
};

#endif /*BUFFER_H*/