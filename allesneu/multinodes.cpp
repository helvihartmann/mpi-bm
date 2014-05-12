#include <iostream>
#include <mpi.h>
#include "parameters.h"
#include "results.h"
#include "buffer.h"

using namespace std;

int main (int argc, char *argv[]){
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    Parameters params(argc, argv);
    int pipelinedepth = params.getpipelinedepth();
    int numberofRootProcesses = params.getnumberofRootProcesses();
    int statisticaliteration = params.getStatisticalIterations();
    int numberofpackages = params.getNumberOfPackageSizes();
    
    Results results(rank, statisticaliteration, numberofpackages);
    Buffer buffer(size, rank, pipelinedepth, numberofRootProcesses, params.getBuffersize());
    
    for (int m = 0; m <= statisticaliteration; m++){
        for (int z = 0; z < numberofpackages; z++){
            size_t packagesize = params.getPackageSizes().at(z);
            size_t packacount = packagesize/sizeof(int);
            size_t innerRuntimeIterations = params.getinnerRuntimeIterations(z, m);
            
            cout << "package size: " << packagesize << ", iterations: " << innerRuntimeIterations << endl;
        }
    }
    
    MPI_Finalize();
    return 0;
}