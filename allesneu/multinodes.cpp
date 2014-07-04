#include <iostream>
#include <mpi.h>
#include "parameters.h"
#include "results.h"
#include "buffer.h"
#include <unistd.h>
#include "tsc.h"

using namespace std;

int main (int argc, char *argv[]){
    
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
    
    double numberofRootProcesses = (double)params.getnumberofRootProcesses();
    double numberofReceivers = (size - numberofRootProcesses);
    unsigned int pipelinedepth = params.getpipelinedepth();
    int pipeline = params.getpipeline();
    int statisticaliteration = params.getStatisticalIterations();
    int numberofpackages = params.getNumberOfPackageSizes();
    int histcheck = params.gethistcheck();
    
    //set flag for ranks if they are sender or receiver----------------------------
    vector<int>sender_vec;
    vector<int>receiver_vec;
    int commflag; //decides wether process is sender (0) or receiver (1)
    for (int rank_index = 0; rank_index < size; rank_index++){
        if (rank_index < numberofRootProcesses){
        //if(rank_index%2 == 0){ //in case of two processes per node
            sender_vec.push_back(rank_index);
            
            if (rank == rank_index){
                cout << "I am sender " << rank_index << endl;
                commflag = 0;
            }
        }
        else{
            receiver_vec.push_back(rank_index);
            if (rank == rank_index){
                cout << "I am receiver " << rank_index << endl;
                commflag = 1;
            }
        }
    }
    // iniate classes
    Results results(rank, statisticaliteration, numberofpackages);
    Buffer buffer(size, rank, pipelinedepth, pipeline, numberofRootProcesses, params.getBuffersize(), sender_vec, receiver_vec);
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
                
                // set up innerRuntimeIterations
                if (numberofRootProcesses <= numberofReceivers){
                    innerRuntimeIterations = innerRuntimeIterations * (numberofRootProcesses/numberofReceivers);
                }
                else{
                    innerRuntimeIterations = innerRuntimeIterations * (numberofReceivers/numberofRootProcesses);
                }
                //innerRuntimeIterations = 12;
                
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
                
                // Header
                time_t Zeitstempel;
                tm *nun;
                Zeitstempel = time(0);
                nun = localtime(&Zeitstempel);
                std::cout << "#" << nun->tm_mday << '.' << nun->tm_mon+1 << '.'<< nun->tm_year+1900 << " - " << nun->tm_hour << ':' << nun->tm_min << std::endl;
                
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
    return 0;
}