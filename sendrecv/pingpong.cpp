#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include "print.h"
#include "parameters.h"
#include "buffer.h"
#include "results.h"
#include "tsc.h"
#include <vector>
using namespace std;

/* fles:~/benchmark/sendrecv 30.01.14
 this is the main.cpp (pingpong.cpp) to perform a MPI benchmark where process 0 sends data to process 1 and gets it back, for which the time is meassured in order to calculate the data rate*/

int main(int argc,char *argv[]){
    
    /*--------------------------------------start MPI-----------------------------*/
    int size, rank, length;
    char name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &length);
    
    cout << "# Prozess " << rank << " von " <<size<<" on "<< name<<" \n";

    /*--------------------- read in parameters-----------------------------*/
    Parameters params;
    params.readOptions(argc,argv);
    int sendmode = params.getsendmode(); // 1 Send, 2 Ssend, 3 Bsend
    
    //------- initaliz stuff for later -------------------
    Results results(params.getStatisticalIterations(), params.getNumberOfPackageSizes());
    
    double starttime, endtime;
    size_t everythingcorrect_check = 0;
    
    //----------------------------- MEASUREMENT --------------------------------------
    //----------------------------- outer statistical iteration loop-------------------
    
    Buffer buffer(sendmode, rank,params.getBuffersize());
    for(int m = 0; m < params.getStatisticalIterations(); m++){
        
        cout<<"# Statistical Iteration cycle "<<m<<"\n";
        
        
        TimeStampCounter timeStampCounter;
        unsigned long long cycle;
        
        //-------------------------iterate over package size-------------------
        for(size_t z = 0; z < params.getNumberOfPackageSizes(); ++z) {
            size_t p = params.getPackageSizes().at(z);
            size_t innerRuntimeIterations = params.getinnerRuntimeIterations(p);
                
                //Process 0 sends the data and gets it back
                if (rank == 0) {
                    
                    results.setvectors(p, innerRuntimeIterations, z);
                    
                    buffer.setloopvariables(p, innerRuntimeIterations, 1);
                    
                    starttime =MPI_Wtime();
                    
                    //double starttime_inner, endtime_inner;
                    timeStampCounter.start();
                    for(size_t j=0; j<innerRuntimeIterations; j++){
                        ///starttime_inner =MPI_Wtime();
                        //timeStampCounter.start();
                        buffer.sendBuffer(j);
                        buffer.recvBuffer(j);
                        //endtime_inner = MPI_Wtime();
                        //timeStampCounter.stop();
                        //cycle = timeStampCounter.cycles();
                        //cout << "# j: "<< j << " timestampcounter: " << timeStampCounter.cycles() << " cycle/3.6GHz: " << 0.5*cycle/3600 << " us" << endl;
                        //cout << j << " p: "<< p <<" mpi_wtime: "<< (endtime_inner-starttime_inner)/2 << " - datarate: " << p/((endtime_inner-starttime_inner)/2) << endl;
                    }
                    endtime = MPI_Wtime();
                    timeStampCounter.stop();
                    cycle = timeStampCounter.cycles();
                    cout <<" timestampcounter: " << timeStampCounter.cycles() << " cycle/3.6GHz: " << cycle/3600 << " us" << endl;
                    
                    if(m==1){
                        results.settime(m,z,((endtime-starttime)/2));
                    }
                    

                 }
                
                //Process 1 receives the data and sends it back
                else if (rank == 1) {
                    
                    buffer.setloopvariables(p, innerRuntimeIterations, 0);
                    starttime =MPI_Wtime();
                    for(size_t j=0; j<innerRuntimeIterations; j++){
                        buffer.recvBuffer(j);
                        buffer.sendBuffer(j);
                    }
                    endtime = MPI_Wtime();
                    
                    buffer.checkBuffer(&everythingcorrect_check);
                    
                    results.settime(m,z,((endtime-starttime)/2));
                }//else if
        }
        cout<<"\n";
    }//for iterations to get statistic errors
    
    /*------------------------------------ OUTPUT -------------------------------*/
    MPI_Barrier(MPI_COMM_WORLD);
    Printoutput out;
    
    // maybe input for rank =1 to send everything to process 0 to compare results...

    if (rank == 0) {

        if(everythingcorrect_check==0){
            
            // Header
            out.printtimestemp();
            out.printheader();
            
            results.calculate();
        }//if everything correct
        
        /*else{
            out.printerrormessage(everythingcorrect_check,p);
        }*/
    }//if you are process 0
    MPI_Finalize();
}