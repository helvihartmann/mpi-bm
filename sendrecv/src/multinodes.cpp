#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <stdio.h>
#include "parameters.h"
#include "buffer.h"
#include "results.h"
#include <vector>
#include <unistd.h>
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
    
    /*--------------------- read in parameters-----------------------------*/
    Parameters params;
    params.readOptions(argc,argv,rank);
    int sendmode = params.getsendmode(); // 1 Send, 2 Ssend, 3 Bsend
    int recvmode = params.getrecvmode();
    size_t numberofcalls = params.getnumberofcalls();
    size_t numberofwarmups = params.getnumberofwarmups();
    int numberofRootProcesses = params.getnumberofRootProcesses();
    
    //------- initaliz stuff for later -------------------
    Results results(params.getStatisticalIterations(), params.getNumberOfPackageSizes());
    
    double starttime, endtime;
    double totaltime;
    int counter = 1;
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
            size_t innerRuntimeIterations;
            if(m == 0){
                innerRuntimeIterations = numberofwarmups;
            }
            else{
                innerRuntimeIterations = params.getinnerRuntimeIterations(p);;
            }
        
            results.setvectors(p, innerRuntimeIterations, z);
            
            buffer.setloopvariables(packageCount, innerRuntimeIterations);
            //Rootprocess send the data
            if (rank < numberofRootProcesses) {
                MPI_Barrier(MPI_COMM_WORLD);
                starttime = MPI_Wtime();
                for (int i = numberofRootProcesses; i < size; i++) {
                    buffer.sendBuffer(i);//send innerRuntimeIterations times
                }
                MPI_Barrier(MPI_COMM_WORLD);
                endtime = MPI_Wtime();
                
                totaltime = (endtime-starttime)/(size-numberofRootProcesses);
                //totaltime = (endtime-starttime)/(size - numberofRootProcesses);//consider full amount of Data sent to all processes (packagsize * number of receivers)
                if(m!=0){
                    results.settime((m-1), z, totaltime);
                }
                else {
                    if(z == 0){
                        cout << "# processes " << size << endl;
                        cout << "# data sent to "  << size << " processes warumup" << endl;
                    }
                    cout << (p*innerRuntimeIterations) << " " << innerRuntimeIterations << " " << p << " " << time << " - " << (p*innerRuntimeIterations)/totaltime << " - " << size << endl;
                }
            }
            
                    //buffer.checkBuffer(&everythingcorrect_check);
 
                //Process 1 receives the data and sends it back
                else {
                    //cout << "# Process " << rank << " receiving" << endl;
                    MPI_Barrier(MPI_COMM_WORLD);
                    starttime =MPI_Wtime();
                    for (int i = 0; i < numberofRootProcesses; i++) {
                        buffer.recvBuffer(i);//send innerRuntimeIterations times
                    }
                    MPI_Barrier(MPI_COMM_WORLD);
                    endtime = MPI_Wtime();
                    totaltime = (endtime - starttime)/(numberofRootProcesses);
                     if(m!=0){
                         results.settime((m-1), z, totaltime);
                    }

                }//else

        }
    MPI_Barrier(MPI_COMM_WORLD);
    cout<<"\n";
    }//for iterations to get statistic errors
    
    /*------------------------------------ OUTPUT -------------------------------*/
    
    // maybe input for rank =1 to send everything to process 0 to compare results...

    for (int i=0; i<size; i++) {
        if (rank == i){
            if(rank == 0){
                
                // Header
                time_t Zeitstempel;
                tm *nun;
                Zeitstempel = time(0);
                nun = localtime(&Zeitstempel);
                std::cout << "#" << nun->tm_mday << '.' << nun->tm_mon+1 << '.'<< nun->tm_year+1900 << " - " << nun->tm_hour << ':' << nun->tm_min << std::endl;
            
                
                cout << "#----------------------- SENDER ---------------------------" << endl;
                cout << "# totaldatasent repeats  packagesize time [us] std sendbandwidth [MB/s] std \n" << endl;
                results.calculate(rank);
                cout << "\n\n" << endl;
            }
            
            else if ( rank < numberofRootProcesses){
                results.calculate(rank);
                cout << "\n\n" << endl;
            }
            
            else {
                cout << "#--- RECEIVER ----------------------------" << endl;
                cout << " number of receivers = " << size - numberofRootProcesses << endl;
                results.calculate(rank);
                cout << "\n\n" << endl;
            }
            sleep(5);
        }
        MPI_Barrier(MPI_COMM_WORLD);

    }
    MPI_Finalize();
}