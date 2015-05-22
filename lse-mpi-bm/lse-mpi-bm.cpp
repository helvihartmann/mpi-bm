#include <iostream>
#include <mpi.h>
#include <memory>
#include "parameters.h"
#include "results.h"
#include "buffer.h"
#include "measurement.h"
#include "output.h"
#include <unistd.h>
#include "tsc.h"

using namespace std;

void pinning(int commflag, int pinningmode);
void printtimestamp();
MPI_Comm setgroups(unsigned int numbercommprocesses, int rank);

    
int main (int argc, char *argv[]){
    
    // Header
   printtimestamp();
    
    //initiate MPI----------------------------------------------------------------
    int rank, size, length;
    char name[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);
    
    
    
    //Parameter class--------------------------------------
    Parameters params(argc, argv);
    
    //set flag for ranks if they are sender or receiver and pin them
    MPI_Barrier(MPI_COMM_WORLD);
    vector<int>remoterank_vec =  params.getsetremoterankvec(size, rank);
    int commflag = params.getcommflag(); //decides wether process is sender (0) or receiver (1)
    
    cout << "# process " << rank << " on host " << name << " reports for duty with commflag " << commflag << endl;
    MPI_Comm communicators_comm = setgroups(params.getnumberofcommprocesses(), rank);
    
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    
    
    if (commflag <= 1){//invoked in communication 0=sender; 1=receiver; 2=nada
        pinning(commflag, params.getpinningmode());
        

        MPI_Barrier(communicators_comm);
        
        sleep(5);
        
        //get and initialize parameters
        unsigned int pipelinedepth = params.getpipelinedepth();
        unsigned int statisticaliteration = params.getStatisticalIterations();
        int numberofpackages = params.getNumberOfPackageSizes();
        int histcheck = params.gethistcheck();
        unsigned int numberofremoteranks = params.getnumberofremoteranks();
       
        // iniate classes
        Results results(rank, statisticaliteration, numberofpackages);
        Buffer buffer(size, rank, pipelinedepth, params.getBuffersize(), remoterank_vec, numberofremoteranks, communicators_comm);
        Output output(rank, size, communicators_comm);
        
        
    
        // do Measurement--------------------------------------------------------------------------
        // repeat measurement couples of times
        for (unsigned int m = 0; m < statisticaliteration; m++){
            
            unique_ptr <Measurement> measurement = nullptr; //initilazided automatically as nullptr only here for ausführlichkeit
            if (commflag == 0){ //sender
                measurement.reset(new Measurementsend(communicators_comm));
            }
            else{//receiver
                measurement.reset(new Measurementrecv(communicators_comm));
            }

            //Warmup
            MPI_Barrier(communicators_comm);
            measurement->warmup(&buffer, params.getnumberofwarmups(), params.getendpackagesize(),rank);
            
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
                        //measurement.measure_hist(packacount,innerRuntimeIterations);
                        measurement->measure(&buffer, packacount,innerRuntimeIterations,hist);

                        if (packagesize >= 8192 && packagesize <= 16384){
                            buffer.printsingletime();
                        }
                        break;
                        
                    default:
                        measurement->measure(&buffer, packacount,innerRuntimeIterations,basic);
                        break;
                }

                
                
                //Write time-----------------------------------------------------------------
                results.setvectors(m, z, innerRuntimeIterations, packagesize, numberofremoteranks,(measurement->getendtime()-measurement->getstarttime()));

            }//z
            
            output.outputiteration(&results, m);
        }//m
        
        //-----------------------------------Output-------------------------------------------------------------
        output.outputfinal(&results, commflag);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    printtimestamp();
    return 0;
}