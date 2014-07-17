#include <iostream>
#include <mpi.h>
#include "parameters.h"
#include "results.h"
#include "buffer.h"
#include <unistd.h>
#include "tsc.h"
#include "timestamp.h"

using namespace std;

int main (int argc, char *argv[]){
    
    Timestamp timestamp;
    timestamp.printtimestamp();
    // Header
    
    
    //initiate MPI----------------------------------------------------------------
    int rank, size, length;
    char name[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);
    
    cout << "# process " << rank << " on host " << name << " reports for duty" << endl;
    
    //get parameters from corresponding class--------------------------------------
    Parameters params(argc, argv);
    
    unsigned int pipelinedepth = params.getpipelinedepth();
    int pipeline = params.getpipeline();
    int statisticaliteration = params.getStatisticalIterations();
    int numberofpackages = params.getNumberOfPackageSizes();
    int histcheck = params.gethistcheck();
    
    //set flag for ranks if they are sender or receiver----------------------------
    MPI_Barrier(MPI_COMM_WORLD);
    params.sendrecvvector(size, rank);
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(5);
    int numberofRootProcesses = params.getnumberofRootProcesses();
    int numberofReceivers = params.getnumberofReceivers();
    vector<int>sender_vec = params.getsendervec();
    vector<int>receiver_vec = params.getrecvvec();
    int commflag = params.getcommflag(); //decides wether process is sender (0) or receiver (1)
    
    // iniate classes
    Results results(rank, statisticaliteration, numberofpackages);
    Buffer buffer(size, rank, pipelinedepth, pipeline, params.getBuffersize(), sender_vec, receiver_vec, numberofRootProcesses, numberofReceivers);
    int numberofRemotranks;
    double starttime, endtime;
    

    for (int m = 0; m <= statisticaliteration; m++){
        //-------------------------------------Warmup--------------------------------------------------
        if (m == 0){
            for (size_t packagecount = 1; packagecount < 1<<24; packagecount = packagecount*2){
                buffer.setloopvariables(packagecount, params.getnumberofwarmups());
                if (commflag == 0){
                    buffer.sendbuffer();
                }
                else{
                    buffer.receivebuffer();
                }
            }
        }
         //Iterate over packagesize ------------------------------------------------------------------------------------
        else{
            for (int z = 0; z < numberofpackages; z++){
                // get loop variables----------------------------------------------------------------------------------
                size_t packagesize = params.getPackageSizes().at(z);
                size_t packacount = packagesize/sizeof(int);
                size_t innerRuntimeIterations = params.getinnerRuntimeIterations(z);
                
                //innerRuntimeIterations = 15;
                
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
                    numberofRemotranks = numberofRootProcesses;
                }
                
                //Write time-----------------------------------------------------------------
                results.setvectors((m-1), z, innerRuntimeIterations, packagesize, numberofRemotranks,(endtime-starttime),buffer.getcyclescomm(),buffer.gettestwaitcounter(),pipelinedepth);
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
            
            MPI_Barrier(MPI_COMM_WORLD);
            for (int i=0; i<size; i++) {
                if (rank == i){
                    if (rank == 0){
                        cout << m << ". iteration-------------------" << endl;
                        
                    }
                    results.printstatisticaliteration();
                    sleep(2);
                }
                MPI_Barrier(MPI_COMM_WORLD);
            }
            
        }
    }//m
    
    //-----------------------------------Output----------------------------------------------------------------------------------------
    //-----------------------------------Output----------------------------------------------------------------------------------------
    for (int i=0; i<size; i++) {
        if (rank == i){
            if(rank == 0){
                
                cout << "# processes " << size << endl;
            }
            
            if (commflag == 0){
                cout << "#----------------------- SENDER ---------------------------" << endl;
                cout << "# totaldatasent repeats  packagesize time [us] std sendbandwidth [MB/s] std \n" << endl;
                results.calculate();
                cout << "\n\n" << endl;
                
                            }
            
            else {
                cout << "#--- RECEIVER ----------------------------" << endl;
                cout << " number of receivers = " << numberofReceivers << endl;
                results.calculate();
                cout << "\n\n" << endl;
            }
            sleep(5);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        
    }

    MPI_Finalize();
    timestamp.printtimestamp();
    return 0;
}