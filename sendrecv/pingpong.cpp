#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <stdio.h>
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
    int recvmode = params.getrecvmode();
    size_t numberofcalls = params.getnumberofcalls();
    
    //------- initaliz stuff for later -------------------
    Results results(params.getStatisticalIterations(), params.getNumberOfPackageSizes());
    
    double starttime, endtime;
    size_t everythingcorrect_check = 0;
    
    //----------------------------- MEASUREMENT --------------------------------------
    //----------------------------- outer statistical iteration loop-------------------
    
    Buffer buffer(sendmode, recvmode, numberofcalls, rank, params.getBuffersize());
    for(int m = 0; m <= params.getStatisticalIterations(); m++){//minimum two iterations m=0 warm up and m=1 first measurement
        
        cout<<"# Statistical Iteration cycle "<<m<<"\n";
                
        //-------------------------iterate over package size-------------------
        for(size_t z = 0; z < params.getNumberOfPackageSizes(); ++z) {
            size_t p = params.getPackageSizes().at(z);
            size_t packageCount = p/sizeof(int);
            size_t innerRuntimeIterations = params.getinnerRuntimeIterations(p);
                
                //Process 0 sends the data and gets it back
                if (rank == 0) {
                    
                    results.setvectors(p, innerRuntimeIterations, z);
                    
                    buffer.setloopvariables(p, innerRuntimeIterations, 1);
                    
                    starttime = MPI_Wtime();
                    
                    for(size_t j=0; j<innerRuntimeIterations; j++){
                        buffer.sendBuffer(j);
                        //buffer.recvBuffer(j);
                    }
                    endtime = MPI_Wtime();
                    
                    if(m!=0){
                        results.settime((m-1), z, ((endtime-starttime)));
                    }
                    else {
                        cout << z << " packagesize " << p << " time " << ((endtime-starttime)/2) << " rate " << (innerRuntimeIterations*p)/(((endtime-starttime))*1000000) << endl;
                    }
                   
                    buffer.checkBuffer(&everythingcorrect_check);
                 }
                
                //Process 1 receives the data and sends it back
                else if (rank == 1) {
                    
                    buffer.setloopvariables(p, innerRuntimeIterations, 0);
                    starttime =MPI_Wtime();
                    for(size_t j=0; j<innerRuntimeIterations; j++){
                        buffer.recvBuffer(j);
                        //buffer.sendBuffer(j);
                    }
                    endtime = MPI_Wtime();
                     
                     if(m!=0){
                     results.settime((m-1), z, ((endtime-starttime)/2));
                     }

                }//else if
        }
        cout<<"\n";
    }//for iterations to get statistic errors
    
    /*------------------------------------ OUTPUT -------------------------------*/
    MPI_Barrier(MPI_COMM_WORLD);
    Printoutput out;
    
    // maybe input for rank =1 to send everything to process 0 to compare results...

    if (rank == 0) {

        if(everythingcorrect_check == 0){
            
            // Header
            out.printtimestemp();
            out.printheader();
            
            results.calculate();
            
        }//if everything correct
        
        else{
            cout<<everythingcorrect_check<<" errors were spotted\n"<<endl;
        }
    }//if you are process 0
    MPI_Finalize();
}