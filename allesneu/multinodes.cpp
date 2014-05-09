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
    
    Parameters param(argc, argv);
    
    Results results(rank);
    Buffer buffer(rank);
    
    MPI_Finalize();
    return 0;
}