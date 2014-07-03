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
    int pipeline;
    int numberofRootProcesses;
    int numberofReceivers = size - numberofRootProcesses;
    size_t buffersize;
    std::vector<int>sender_vec;
    std::vector<int>receiver_vec;
    int *buffer;
    
    size_t innerRuntimeIterations;
    size_t packagecount;
    
    std::vector<unsigned long long>sendstart;
    std::vector<unsigned long long>sendstop;
    std::vector<unsigned long long>waitstart;
    std::vector<unsigned long long>waitstop;
    std::vector<unsigned long long>recvstart;
    std::vector<unsigned long long>recvstop;
    std::vector<unsigned long long>waitrstart;
    std::vector<unsigned long long>waitrstop;
    
    TimeStampCounter timestamp;
    
    
public:
    
    Buffer(int size, int rank, unsigned int pipelinedepth, int pipeline, int numberofrootprocesses, size_t buffersize, std::vector<int>sender_vec, std::vector<int>receiver_vec);
    
    void setloopvariables(size_t packagecount, size_t innerRuntimeIterations);
    void sendbuffer();
    
    void receivebuffer();
    
    void printsingletime();
};

#endif /*BUFFER_H*/