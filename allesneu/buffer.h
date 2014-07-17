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
    size_t buffersize;
    std::vector<int>sender_vec;
    std::vector<int>receiver_vec;
    
    int numberofRootProcesses; // now every process has his own
    int numberofReceivers;
    
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
    
    std::vector<unsigned long long>cycles_comm;
    std::vector<size_t>testwaitcounter;
    
    
public:
    
    Buffer(int size, int rank, unsigned int pipelinedepth, int pipeline, size_t buffersize, std::vector<int>sender_vec, std::vector<int>receiver_vec, int numberofRootProcesses, int numberofReceivers);
    
    void setloopvariables(size_t packagecount, size_t innerRuntimeIterations);
    void sendbuffer();
    
    void receivebuffer();
    
    void printsingletime();
    
    std::vector<unsigned long long> getcyclescomm() { return cycles_comm; }
    
    std::vector<size_t> gettestwaitcounter() { return testwaitcounter; }
};

#endif /*BUFFER_H*/