#include <iostream>
#include <mpi.h>
#include <memory>
#include "parameters.h"
#include "results.h"
#include "measurement.h"
//#include "output.h"
#include <unistd.h>
#include "tsc.h"

using namespace std;

/*2015 Copyright Helvi Hartmann <hhartmann@fias.uni-frankfurt.de>
 Main for the LSE (Linear Shift Exchange Communication) Pattern used to measure the data rate of an all-to-all communication between n%2==0 processes started by a slurm script, which is created via runbm.sh
 */

void pinning(int commflag, int pinningmode);
void printtimestamp();
MPI_Comm setgroups(unsigned int numbercommprocesses, int rank);

    
int main (int argc, char *argv[]){
    
    // Header
    printtimestamp();
    
    //initiate MPI-----------------------------------------------------------------------------------------------
    int rank, size, length;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);
    
    //Parameter class
    Parameters params(argc, argv);
    
    //set flag for ranks if they are sender or receiver-----------------------------------------------------------
    MPI_Barrier(MPI_COMM_WORLD);
    vector<int>remoterank_vec =  params.getsetremoterankvec(size, rank);
    int commflag = params.getcommflag(); //decides wether process is sender (0) or receiver (1)
    cout << "# process " << rank << " on host " << name << " reports for duty with commflag " << commflag << endl;
    MPI_Comm communicators_comm = setgroups(params.getnumberofcommprocesses(), rank);
    MPI_Barrier(MPI_COMM_WORLD);
    
    //major part only for process which are invoked in communication 0=sender; 1=receiver; 2=nada-----------------
    if (commflag <= 1){
        //pinning sender and receiver to correct numa node
        pinning(commflag, params.getpinningmode());
        MPI_Barrier(communicators_comm);
        
        //get and initialize parameters
        unsigned int pipelinedepth = params.getpipelinedepth();
        unsigned int statisticaliteration = params.getStatisticalIterations();
       
        // iniate classes
        Results results(rank, statisticaliteration, params.getPackageSizes().size(), communicators_comm, size);
        //Output output(rank, size, communicators_comm);
        //point to correct function depending on if process is sender or receiver
        unique_ptr <Measurement> measurement = nullptr;
        if (commflag == 0){ //sender
            measurement.reset(new Measurementsend(params.getBuffersize(), communicators_comm));
        }
        else{//receiver
            measurement.reset(new Measurementrecv(params.getBuffersize(), communicators_comm));
        }
    
        // do Measurement----------------------------------------------------------------------------------------
        // repeat measurement couples of times for statistics
        for (unsigned int m = 0; m < statisticaliteration; m++){

            //Warmup
            MPI_Barrier(communicators_comm);
            
            //Data rate measurement: Iterate over packagesize-----------------------------------------------------
            std::vector<size_t> warmups(params.getPackageSizes().size(), params.getnumberofwarmups());
            measurement->measure(warmups, params.getPackageSizes(), remoterank_vec, rank, 1, 0, &results, m);
            measurement->measure(params.getinnerRuntimeIterations(), params.getPackageSizes(), remoterank_vec, rank, pipelinedepth, 1, &results, m);
            results.outputiteration(m);
        }//m statisticcal iteration
        
        //-----------------------------------Output-------------------------------------------------------------
        results.outputfinal(commflag);
    }//if communicating process
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    printtimestamp();
    return 0;
}