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
    unsigned int pipelinedepth;
    int numberofRootProcesses;
    int statisticaliteration;
    size_t factor;
    size_t factor_fix;
    size_t buffersize;
    size_t startpackagesize;
    size_t endpackagesize;
    
    std::vector<size_t> packageSizes;
    
public:
    
    Parameters(int argc, char **argv);
    
    const std::vector<size_t>& getPackageSizes() { return packageSizes; }
    
    size_t getNumberOfPackageSizes() { return packageSizes.size(); }
    
    int getStatisticalIterations() { return statisticaliteration; }
    
    size_t getBuffersize() { return buffersize; }
    
    unsigned int getpipelinedepth() { return pipelinedepth; }
    
    int getnumberofRootProcesses() { return numberofRootProcesses; }

    size_t getinnerRuntimeIterations(int z, int m);
    
};

#endif /*PARAMETERS_H*/