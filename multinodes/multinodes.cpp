#include <iostream>
#include <mpi.h>
#include "parameters.h"
#include "results.h"
#include "buffer.h"
#include "measurement.h"
#include "output.h"
#include <unistd.h>
#include "tsc.h"
#include "timestamp.h"
#include <numa.h>
using namespace std;

int main (int argc, char *argv[]){
    
    // Header
    Timestamp timestamp;
    timestamp.printtimestamp();
    
    //initiate MPI----------------------------------------------------------------
    int rank, size, length;
    char name[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);
    
    cout << "# process " << rank << " on host " << name << " reports for duty" << endl;
    
    //Pinning Processes--------------------------------------
    /*struct bitmask* nodemask = numa_allocate_cpumask();
    numa_bitmask_setbit(nodemask, 0);
    cout << "pinning to numa_node 0" << endl;
    numa_bind(nodemask);*/
    //numa_set_membind(nodemask);
    
    //Parameter class--------------------------------------
    Parameters params(argc, argv);
    
    //set flag for ranks if they are sender or receiver
    MPI_Barrier(MPI_COMM_WORLD);
    vector<int>remoterank_vec =  params.getsetremoterankvec(size, rank);
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(5);
    
    //get and initialize parameters
    unsigned int pipelinedepth = params.getpipelinedepth();
    unsigned int statisticaliteration = params.getStatisticalIterations();
    int numberofpackages = params.getNumberOfPackageSizes();
    int histcheck = params.gethistcheck();
    unsigned int numberofremoteranks = params.getnumberofremoteranks();
    int commflag = params.getcommflag(); //decides wether process is sender (0) or receiver (1)
    
    int (*mpisend)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*) = MPI_Issend;
    int (*mpirecv)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*) = MPI_Irecv;
    
    // iniate classes
    Results results(rank, statisticaliteration, numberofpackages);
    Buffer buffer(size, rank, pipelinedepth, params.getBuffersize(), remoterank_vec, numberofremoteranks);
    Output output(rank, size);
    
    Measurement measurement(&buffer);
    
    if (commflag == 0){
        measurement.setfunctionpointer(mpisend);
    }
    else{
        measurement.setfunctionpointer(mpirecv);
    }
    
    // do Measurement--------------------------------------------------------------------------
    for (unsigned int m = 0; m < statisticaliteration; m++){
        
        MPI_Barrier(MPI_COMM_WORLD);
        measurement.warmup(params.getnumberofwarmups(), rank);
        
        //Iterate over packagesize----------------------------------------------------------------------------
        for (int z = 0; z < numberofpackages; z++){
            
            // get loop variables-----------------------------------------------------------------------------
            size_t packagesize = params.getPackageSizes().at(z);
            size_t packacount = packagesize/sizeof(int);
            size_t innerRuntimeIterations = params.getinnerRuntimeIterations(z);
            if (pipelinedepth > innerRuntimeIterations){
                pipelinedepth = innerRuntimeIterations-2;
            }
                        
            switch (histcheck) {//basically the same but case1 prints additonally files with times for every single meassurement for a packagesize of 16kiB where stuff usually goes wrong
                case 1:
                    measurement.measure_hist(packacount,innerRuntimeIterations);
                    if (packagesize >= 8192 && packagesize <= 16384){
                        buffer.printsingletime();
                    }
                    break;
                    
                default:
                    measurement.measure(packacount,innerRuntimeIterations);
                    break;
            }
            
            //Write time-----------------------------------------------------------------
            results.setvectors(m, z, innerRuntimeIterations, packagesize, numberofremoteranks,(measurement.getendtime()-measurement.getstarttime()),buffer.gettestwaitcounter(),pipelinedepth);

        }//z
        
        output.outputiteration(&results, m);
    }//m
    
    //-----------------------------------Output-------------------------------------------------------------
    output.outputfinal(&results, commflag);


    MPI_Finalize();
    timestamp.printtimestamp();
    return 0;
}