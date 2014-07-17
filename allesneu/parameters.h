#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <iostream>
#include <vector>
#include <unistd.h>
#include <getopt.h>
/**/

class Parameters{
private:
    int numberofwarmups;
    int multicore;
    unsigned int pipelinedepth;
    int pipeline;
    int numberofRootProcesses;
    int numberofReceivers;
    int statisticaliteration;
    size_t factor;
    size_t factor_fix;
    size_t buffersize;
    int histcheck;
    size_t startpackagesize;
    size_t endpackagesize;
    
    std::vector<size_t> packageSizes;
    
    std::vector<int>sender_vec;
    std::vector<int>receiver_vec;
    
    int commflag;
public:
    
    Parameters(int argc, char **argv);
    
    const std::vector<size_t>& getPackageSizes() { return packageSizes; }
    
    size_t getNumberOfPackageSizes() { return packageSizes.size(); }
    
    int getStatisticalIterations() { return statisticaliteration; }
    
    size_t getBuffersize() { return buffersize; }
    
    int gethistcheck() { return histcheck; }
    
    unsigned int getpipelinedepth() { return pipelinedepth; }
    
    int getpipeline() { return pipeline; }
    
    int getnumberofRootProcesses() { return numberofRootProcesses; }
    
    int getnumberofReceivers() { return numberofReceivers; }
    
    int getnumberofwarmups() { return numberofwarmups; }
    
    std::vector<int> getsendervec() { return sender_vec; }
    
    std::vector<int> getrecvvec() { return receiver_vec; }
    
    int getcommflag() { return commflag; }
    
    size_t getinnerRuntimeIterations(int z, int size);
    
    void sendrecvvector(int size, int rank);
    
};

#endif /*PARAMETERS_H*/