#include <mpi.h>
#include <memory>
#include <vector>
#include "dataManager.h"
#include "parameters.h"

/*2015 Copyright Helvi Hartmann <hhartmann@fias.uni-frankfurt.de> */

MPI_Comm setgroups(unsigned int numbercommprocesses, int rank);

void output(int rank, unsigned int nmbr_commprocess, std::vector<double> time, std::vector<size_t> packagesize, std::vector<size_t> innerRuntimeIterations);
void printtimestamp();

int main (int argc, char *argv[]){
    //initiate MPI-----------------------------------------------------------------------------------------------
    int rank, size, length;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);
   
    // Header
    printtimestamp();
    cout << "# process " << rank << " on host " << name << " reports for duty" << endl;

    
    Parameters params(argc, argv);
    std::vector<size_t>packagesize = params.getpackagesizes();
    std::vector<size_t>innerRuntimeIterations = params.getinnerRuntimeIterations();
    int commflag = params.getsetflag(rank);
    std::vector<int>remoteranks = params.getsetremoterankvec(size, rank);
    
    //point to correct function depending on if process is sender or receiver
    unique_ptr <DataManager> measurement = nullptr;
    //if (rank%2 == 0){ //sender
    if (commflag == 0){
        measurement.reset(new Measurementsend(params.getbuffersize()));
    }
    else if (commflag == 1){//receiver
        measurement.reset(new Measurementrecv(params.getbuffersize()));
    }
    else{
        measurement.reset(new Measurementobserver(params.getbuffersize()));
    }

    //measurement
    std::vector<double>time = measurement->run(packagesize, innerRuntimeIterations, remoteranks, params.getnumberofwarmups(), commflag, params.getnumberofcommprocesses());
    
    output(rank, params.getnumberofcommprocesses(), time, packagesize, innerRuntimeIterations);

    //finalize MPI-----------------------------------------------------------------------------------------------
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}