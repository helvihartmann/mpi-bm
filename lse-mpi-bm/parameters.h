#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <iostream>
#include <vector>
#include <unistd.h>
#include <getopt.h>

/*2015 Copyright Helvi Hartmann <hhartmann@fias.uni-frankfurt.de>
 Parameter class takes in all options parsed in function call or sets default values*/

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
    unsigned int size;
    unsigned int rank;
    size_t factor;
    size_t factor_fix;
    size_t buffersize;
    size_t startpackagesize;
    size_t endpackagesize;
    
    std::vector<size_t> packageSizes;
    
    std::vector<int>remoterank_vec;
    
    int commflag;
    int queue;
    
    unsigned int numberofcommprocesses;
public:
    
    Parameters(int argc, char **argv);
    
    const std::vector<size_t>& getPackageSizes() { return packageSizes; }
    
    size_t getNumberOfPackageSizes() { return packageSizes.size(); }
    
    unsigned int getStatisticalIterations() { return statisticaliteration; }
    
    size_t getBuffersize() { return buffersize; }
        
    unsigned int getpipelinedepth() { return pipelinedepth; }
    
    size_t getnumberofwarmups() { return numberofwarmups; }
    
    unsigned int getnumberofremoteranks() {return numberofremoteranks; }
    
    int getcommflag() { return commflag; }
    
    unsigned int getnumberofcommprocesses() { return numberofcommprocesses; }
    
    size_t getinnerRuntimeIterations(int z);
    
    size_t getendpackagesize() { return endpackagesize; }
    
    std::vector<int> getsetremoterankvec(unsigned int size,unsigned int rank);
    
    int getqueue() { return queue; }
    
    int getmulticore() { return multicore; }
    
    int getpinningmode() { return pinningmode; }
        
    void applyLSE(int start, int sign);
    
    void setflag(int commflag_, unsigned int numberofremoteranks_);
    
};

#endif /*PARAMETERS_H*/