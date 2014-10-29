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
    unsigned int size;
    unsigned int rank;
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
    enum Flag {on, off};
    Flag barrelshiftingflag;
public:
    
    Parameters(int argc, char **argv);
    
    const std::vector<size_t>& getPackageSizes() { return packageSizes; }
    
    size_t getNumberOfPackageSizes() { return packageSizes.size(); }
    
    unsigned int getStatisticalIterations() { return statisticaliteration; }
    
    size_t getBuffersize() { return buffersize; }
    
    int gethistcheck() { return histcheck; }
    
    unsigned int getpipelinedepth() { return pipelinedepth; }
    
    size_t getnumberofwarmups() { return numberofwarmups; }
    
    unsigned int getnumberofremoteranks() {return numberofremoteranks; }
    
    int getcommflag() { return commflag; }
    
    int getnumberofcommprocesses() { return (numberofReceivers + numberofSenders); }
    
    size_t getinnerRuntimeIterations(int z);
    
    size_t getendpackagesize() { return endpackagesize; }
    
    std::vector<int> getsetremoterankvec(unsigned int size,unsigned int rank);
    
    int getqueue() { return queue; }
    
    int getmulticore() { return multicore; }
    
    int getpinningmode() { return pinningmode; }
    
    void sortlist(unsigned int start, unsigned int end, unsigned int except, int increment);
    
    void barrelshifting(int start, int sign);
    
    void setflag(int commflag_, unsigned int numberofremoteranks_);
    
};

#endif /*PARAMETERS_H*/