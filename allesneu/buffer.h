#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <queue>
#include <mpi.h>
#include "tsc.h"
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;
/**/

class Buffer{
private:
    int size;
    int rank;
    
    unsigned int pipelinedepth;
    int pipeline;
    size_t buffersize;
    std::vector<int>sender_vec;
    std::vector<int>receiver_vec;
    
    unsigned int numberofSenders; // now every process has his own
    unsigned int numberofReceivers;
    
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
    
    size_t errorcounter = 0;
    
    
    
public:
    
    Buffer(int size, int rank, unsigned int pipelinedepth, int pipeline, size_t buffersize, std::vector<int>sender_vec, std::vector<int>receiver_vec, unsigned int numberofSenders, unsigned int numberofReceivers);
   
    void checkbuffer(int value, int remoterank);
    
    void setloopvariables(size_t packagecount, size_t innerRuntimeIterations);
    void sendbuffer();
    
    void receivebuffer();
    
    void printsingletime();
    
    std::vector<unsigned long long> getcyclescomm() { return cycles_comm; }
    
    std::vector<size_t> gettestwaitcounter() { return testwaitcounter; }
    
    size_t geterrorcounter() { return errorcounter; }
};

#endif /*BUFFER_H*/