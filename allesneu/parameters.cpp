#include "parameters.h"

Parameters::Parameters(int argc, char **argv){
    numberofwarmups = 20;
    pipelinedepth = 8;
    numberofRootProcesses = 1;
    statisticaliteration = 1;
    factor = 1<<20;
    buffersize = 1<<30;
    
    
    startpackagesize = 1 << 2;
    endpackagesize = 1 << 20;
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
        if (z == 0){
            std::cout << "#WARMUP" << std::endl;
        }
        innerRuntimeIterations = numberofwarmups;
    }
    else{
        if (z == 0){
            std::cout << "# " << m << ". iteration" << std::endl;
        }
        if (packageSizes.at(z) <= 1000)  {
            innerRuntimeIterations = factor;
        }
        else{
            innerRuntimeIterations = 6000000/packageSizes.at(z);
        }
        if (innerRuntimeIterations <= 5){
            innerRuntimeIterations = 5;
        }
    }
    
    
    return innerRuntimeIterations;
}