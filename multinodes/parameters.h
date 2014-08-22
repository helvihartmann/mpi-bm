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
    int pinningmode;
    unsigned int pipelinedepth;
    unsigned int numberofSenders;
    unsigned int numberofReceivers;
    unsigned int numberofremoteranks;
    unsigned int statisticaliteration;
    size_t factor;
    size_t factor_fix;
    size_t buffersize;
    int histcheck;
    size_t startpackagesize;
    size_t endpackagesize;
    
    std::vector<size_t> packageSizes;
    
    std::vector<int>remoterank_vec;
    
    int commflag;
    int queue;
public:
    
    Parameters(int argc, char **argv);
    
    const std::vector<size_t>& getPackageSizes() { return packageSizes; }
    
    size_t getNumberOfPackageSizes() { return packageSizes.size(); }
    
    unsigned int getStatisticalIterations() { return statisticaliteration; }
    
    size_t getBuffersize() { return buffersize; }
    
    int gethistcheck() { return histcheck; }
    
    unsigned int getpipelinedepth() { return pipelinedepth; }
    
    size_t getnumberofwarmups();
    
    unsigned int getnumberofremoteranks() {return numberofremoteranks; }
    
    int getcommflag() { return commflag; }
    
    size_t getinnerRuntimeIterations(int z);
    
    std::vector<int> getsetremoterankvec(unsigned int size,unsigned int rank);
    
    int getqueue() { return queue; }
    
    int getmulticore() { return multicore; }
    
    int getpinningmode() { return pinningmode; }
    
};

#endif /*PARAMETERS_H*/