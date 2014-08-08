#include <iostream>
#include <mpi.h>
#include "parameters.h"
#include "results.h"
#include "buffer.h"
#include "warmup.h"
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
    struct bitmask* nodemask = numa_allocate_cpumask();
    numa_bitmask_setbit(nodemask, 0);
    cout << "pinning to numa_node 0" << endl;
    numa_bind(nodemask);
    //numa_set_membind(nodemask);
    //Parameter class--------------------------------------
    Parameters params(argc, argv);
    
    //set flag for ranks if they are sender or receiver
    MPI_Barrier(MPI_COMM_WORLD);
    params.sendrecvvector(size, rank);
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(5);
    
    //get and initialize parameters
    unsigned int pipelinedepth = params.getpipelinedepth();
    int pipeline = params.getpipeline();
    unsigned int statisticaliteration = params.getStatisticalIterations();
    int numberofpackages = params.getNumberOfPackageSizes();
    int histcheck = params.gethistcheck();
    unsigned int numberofSenders = params.getnumberofSenders();
    unsigned int numberofReceivers = params.getnumberofReceivers();
    vector<int>sender_vec = params.getsendervec();
    vector<int>receiver_vec = params.getrecvvec();
    int commflag = params.getcommflag(); //decides wether process is sender (0) or receiver (1)
    int numberofRemotranks;
    double starttime, endtime;
    
    // iniate classes
    Results results(rank, statisticaliteration, numberofpackages);
    Buffer buffer(size, rank, pipelinedepth, pipeline, params.getBuffersize(), sender_vec, receiver_vec, numberofSenders, numberofReceivers);
    Output output(rank, size);
    
    // do Measurement--------------------------------------------------------------------------
    for (unsigned int m = 0; m < statisticaliteration; m++){
        MPI_Barrier(MPI_COMM_WORLD);
        Warmup warmup(&buffer, commflag, params.getnumberofwarmups(), rank, numberofSenders, numberofReceivers);
        
        //Iterate over packagesize----------------------------------------------------------------------------
        for (int z = 0; z < numberofpackages; z++){
            
            // get loop variables-----------------------------------------------------------------------------
            size_t packagesize = params.getPackageSizes().at(z);
            size_t packacount = packagesize/sizeof(int);
            size_t innerRuntimeIterations = params.getinnerRuntimeIterations(z);
            if (pipelinedepth > innerRuntimeIterations){
                pipelinedepth = innerRuntimeIterations-2;
            }
            buffer.setloopvariables(packacount, innerRuntimeIterations);
            
            // send-----------------------------------------------------------
            if (commflag == 0){
                MPI_Barrier(MPI_COMM_WORLD);
                starttime = MPI_Wtime();
                buffer.sendbuffer();
                MPI_Barrier(MPI_COMM_WORLD);
                endtime = MPI_Wtime();
                numberofRemotranks = numberofReceivers;
            }
            // receive-------------------------------------------------------
            else{
                MPI_Barrier(MPI_COMM_WORLD);
                starttime = MPI_Wtime();
                buffer.receivebuffer();
                MPI_Barrier(MPI_COMM_WORLD);
                endtime = MPI_Wtime();
                numberofRemotranks = numberofSenders;
            }
            
            //Write time-----------------------------------------------------------------
            results.setvectors(m, z, innerRuntimeIterations, packagesize, numberofRemotranks,(endtime-starttime),buffer.gettestwaitcounter(),pipelinedepth);
            
            switch (histcheck) {
                case 1:
                    if (packagesize >= 8192 && packagesize <= 16384){
                        buffer.printsingletime();
                    }
                    break;
                    
                default:
                    break;
            }
        }//z
        
        output.outputiteration(&results, m);
    }//m
    
    //-----------------------------------Output-------------------------------------------------------------
    output.outputfinal(&results, commflag, numberofReceivers);


    MPI_Finalize();
    timestamp.printtimestamp();
    return 0;
}