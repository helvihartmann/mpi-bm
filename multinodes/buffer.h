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
    size_t buffersize;
    std::vector<int>remoterank_vec;
    unsigned int numberofremoteranks; // now every process has his own
    
    int *buffer;
    
    size_t innerRuntimeIterations;
    size_t packagecount;
    
    std::vector<unsigned long long>commstart;
    std::vector<unsigned long long>commstop;
    std::vector<unsigned long long>waitstart;
    std::vector<unsigned long long>waitstop;

    
    TimeStampCounter timestamp;
    
    std::vector<size_t>testwaitcounter;
    
    size_t errorcounter = 0;
    
    
    
public:
    
    Buffer(int size, int rank, unsigned int pipelinedepth, size_t buffersize, std::vector<int>remoterank_vec, unsigned int numberofremoteranks);
   
    void checkbuffer(int value, int remoterank);
    
    void setloopvariables(size_t packagecount, size_t innerRuntimeIterations);
    
    void comm(int (*mpicall)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
    
    void comm_hist(int (*mpicall)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
    
    void emptyqueue(std::queue<MPI_Request> queue_request);
    
    void printsingletime();
        
    std::vector<size_t> gettestwaitcounter() { return testwaitcounter; }
    
    size_t geterrorcounter() { return errorcounter; }
};

#endif /*BUFFER_H*/