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
    size_t numberofwarmups;
    int multicore;
    int pinningmode;
    unsigned int pipelinedepth;
    unsigned int nmbr_commprocess;
    size_t factor;
    size_t factor_fix;
    size_t buffersize;
    size_t startpackagesize;
    size_t endpackagesize;
    int packageSizeFactor;
    
    std::vector<size_t> packagesizes;
    
    
public:
    
    Parameters(int argc, char **argv);
    
    int getsetflag(int rank);
    
    const std::vector<size_t>& getpackagesizes() { return packagesizes; }
    
    size_t getNumberOfPackageSizes() { return packagesizes.size(); }
    
    size_t getbuffersize() { return buffersize; }
    
    unsigned int getpipelinedepth() { return pipelinedepth; }
    
    size_t getnumberofwarmups() { return numberofwarmups; }
    
    unsigned int getnumberofcommprocesses() { return nmbr_commprocess; }
    
    std::vector<size_t> getinnerRuntimeIterations();
    
    size_t getendpackagesize() { return endpackagesize; }
    
    int getmulticore() { return multicore; }
    
    int getpinningmode() { return pinningmode; }
};

#endif /*PARAMETERS_H*/