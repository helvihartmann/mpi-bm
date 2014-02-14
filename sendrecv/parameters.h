#ifndef PARAMETERS
#define PARAMETERS

#include <cstddef>
#include <cstdlib>
#include <vector>
/* pfad
 14.11.2013
*/

    //get starting packege size; read in data to send from console (default =128B)
class Parameters{
private:
    int sendmode, recvmode; //"MPI_Send" by default
    size_t factor;
    size_t buffersize;
    
    size_t startPackageSize;
    size_t endPackageSize;
    double packageSizeFactor;

    int statisticaliterations;
    std::vector<size_t> packageSizes;
    
    size_t numberofcalls;
    size_t numberofwarmups;

public:
    
    void setPackagesizeTmp(size_t);
    
    void readOptions(int, char**);
    
    size_t getStartPackageSize() { return startPackageSize; }
    
    size_t getEndPackageSize() { return endPackageSize; }
    
    double getPackageSizeFactor() { return packageSizeFactor; }
    
    const std::vector<size_t>& getPackageSizes() { return packageSizes; }
    
    size_t getNumberOfPackageSizes() { return packageSizes.size(); }
    
    int getStatisticalIterations() { return statisticaliterations; }
    
    int getsendmode() { return sendmode; }
    
    int getrecvmode() { return recvmode; }
    
    size_t getBuffersize() { return buffersize; }
    
    size_t getnumberofcalls() { return numberofcalls; }
    
    size_t getnumberofwarmups() { return numberofwarmups; }

/* calculate the number of sending the package to have process occur in seconds regime
 sending 128 5millions time took a reasonable time, thats where the empiricalfactor comes from*/
    size_t getinnerRuntimeIterations(size_t packageSize);
};
#endif
