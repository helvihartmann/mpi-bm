#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <queue>
#include <mpi.h>
#include "tsc.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "measurement.h"

using namespace std;
/**/

class Buffer{
private:
    int size;
    int rank;
    
    unsigned int pipelinedepth;
    size_t buffersize;
    std::vector<int>remoterank_vec;
    unsigned int numberofremoteranks; // now every process has his own
    MPI_Comm communicators_comm;
    
    int *buffer;
    
    size_t innerRuntimeIterations;
    size_t packagecount;
    
    std::vector<unsigned long long>commstart;
    std::vector<unsigned long long>commstop;
    std::vector<unsigned long long>waitstart;
    std::vector<unsigned long long>waitstop;

    int remoterank = -1;
    MPI_Request comm_obj;
    
    size_t index = 0;

    
    TimeStampCounter timestamp;
    
    //std::vector<size_t>testwaitcounter;
    
    size_t errorcounter = 0;
    
    
    
    
public:
    
    Buffer(int size, int rank, unsigned int pipelinedepth, size_t buffersize, std::vector<int>remoterank_vec, unsigned int numberofremoteranks, MPI_Comm communicators_comm_);
   
    void checkbuffer(int value, int remoterank);
    
    void setloopvariables(size_t packagecount, size_t innerRuntimeIterations);
    
    
    void comm(Measurement *measurement);
    
    void comm_hist(Measurement *measurement);
        
    void emptyqueue(std::queue<MPI_Request> queue_request);
    
    void printsingletime();
        
    //std::vector<size_t> gettestwaitcounter() { return testwaitcounter; }
    
    size_t geterrorcounter() { return errorcounter; }
};

#endif /*BUFFER_H*/