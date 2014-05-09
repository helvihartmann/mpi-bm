#include "parameters.h"

Parameters::Parameters(int argc, char **argv){
    numberofwarmups = 10;
    pipelinedepth = 8;
    numberofRootProcesses = 1;
    statisticaliteration = 1;
    factor = 1<<20;
    buffersize = 1<<30;
    
    
    startpackagesize = 1 << 2;
    endpackagesize = 1 << 24;
    int packageSizeFactor = 2;
    if (startpackagesize <= endpackagesize){
        for (size_t p = startpackagesize; p <= endpackagesize; p = p * packageSizeFactor){
                packageSizes.push_back(p);
        }
    }
    else{
        for (size_t p = startpackagesize; p >= endpackagesize; p = p/packageSizeFactor){
                packageSizes.push_back(p);
        }
    }
    
    std::cout<<"#start packagesize " << startpackagesize << ", inner iterations " << factor << ", end packagesize " << endpackagesize << ", statistical iterations " <<statisticaliteration << ", buffersize " << buffersize << ", pipeline depth " << pipelinedepth << ", number of warm ups " << numberofwarmups << ", number of senders " << numberofRootProcesses << std::endl;
}


size_t Parameters::getinnerRuntimeIterations(int z, int m) {
    size_t innerRuntimeIterations;
    
    if (m == 0){
        innerRuntimeIterations = numberofwarmups;
    }
    if (packageSizes.at(z) <= 10000)  {
        innerRuntimeIterations = factor;
    }
    else{
        innerRuntimeIterations = 6000/packageSizes.at(z);
    }
    
    return innerRuntimeIterations;
}