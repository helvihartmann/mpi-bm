#include <mpi.h>
#include <memory>
#include <vector>
#include "dataManager.h"
#include "parameters.h"

/*2015 Copyright Helvi Hartmann <hhartmann@fias.uni-frankfurt.de> */

MPI_Comm setgroups(unsigned int numbercommprocesses, int rank);

void output(int rank, int size, std::vector<double> time, std::vector<size_t> packagesize, std::vector<size_t> innerRuntimeIterations);

int main (int argc, char *argv[]){
//initiate MPI-----------------------------------------------------------------------------------------------
    int rank, size, length;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);
   
    cout << "# process " << rank << " on host " << name << " reports for duty" << endl;
    size_t buffersize = 17179869184;

    Parameters params(argc, argv);
    std::vector<size_t>packagesize = params.getpackagesizes();
    std::vector<size_t>innerRuntimeIterations = params.getinnerRuntimeIterations();


    //point to correct function depending on if process is sender or receiver
    unique_ptr <DataManager> measurement = nullptr;
    if (rank%2 == 0){ //sender
        measurement.reset(new Measurementsend(buffersize));
    }
    else{//receiver
        measurement.reset(new Measurementrecv(buffersize));
    }

    //measurement
    std::vector<double>time = measurement->run(packagesize, size, innerRuntimeIterations, rank, params.getnumberofwarmups());
    
    output(rank, size, time, packagesize, innerRuntimeIterations);


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}

