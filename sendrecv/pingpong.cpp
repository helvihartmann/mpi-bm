#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include "classmpi3.h"
#include "print.h"
#include "parameters.h"
#include "buffer.h"
#include "results.h"
#include <vector>
using namespace std;

/* fles:~/benchmark/sendrecv 27.11.2013*/

int main(int argc,char *argv[]){
    
    /*--------------------------------------start MPI-----------------------------*/
    int size, rank;
    int length;
    char name[MPI_MAX_PROCESSOR_NAME];
    //Mpi mpi1;
    Mpi *mpi1pnter = new Mpi;
    //mpi1pnter = &mpi1;
    
    mpi1pnter->init_it(&argc,&argv);
    size = mpi1pnter->get_size();
    rank = mpi1pnter->get_rank();
    MPI_Get_processor_name(name, &length); //not yet handled in class
    

    /*--------------------- Iterate over packege size-----------------------------*/
    //get starting packege size; read in data to send from console (default =128B)
    Parameters params;
    params.readOptions(argc,argv);
    cout << "# Prozess " << rank << " von " <<size<<" on "<< name<<" \n";
    
    int sendmode = params.getsendmode(); // 1 Send, 2 Ssend, 3 Bsend
    
    double starttime, endtime;
 
    //-------Vector definations-------------------
    Results results(params.getStatisticalIterations(), params.getNumberOfPackageSizes());
    
    size_t everythingcorrect_check = 0;
    
#if (1)
    for(int m = 0; m < params.getStatisticalIterations(); m++){
        
        cout<<"# Statistical Iteration cycle "<<m<<"\n";
        
        Buffer buffer(sendmode, *mpi1pnter, rank);
        
        for(size_t z = 0; z < params.getNumberOfPackageSizes(); ++z) {
            size_t p = params.getPackageSizes().at(z);
            
            /* -------------- make first calculations on datavolume---------------------------*/
            size_t innerRuntimeIterations = params.getinnerRuntimeIterations(p);
            
            /*----------------------repeadingly send the package---------------------*/
                
                //Process 0 sends the data
                if (rank == 0) {
                    
                    buffer.setloopvariables(p, innerRuntimeIterations, 1);

                    results.setvectors(p, innerRuntimeIterations, z);
                    
                    // time measure sending process
                    starttime = mpi1pnter->get_mpitime();
                    buffer.sendBuffer();
                    buffer.recvBuffer();
                    endtime = mpi1pnter->get_mpitime();
                    results.settime(m,z,((endtime-starttime)/2));
                    
                    //systemload
                    //int nelem=3;
                    //double loadavg[nelem];
                    //int systemload = getloadavg(loadavg, nelem);
                    //getloadavg(loadavg, nelem);
                    //loadavg_vector.at(z)=loadavg[1];
                 }
                
                //Process 1 receives the data
                else if (rank == 1) {
                    
                    buffer.setloopvariables(p, innerRuntimeIterations, 0);
                    
                    //time measure receving data
                    starttime = mpi1pnter->get_mpitime();
                    buffer.recvBuffer();
                    buffer.sendBuffer();
                    endtime = mpi1pnter->get_mpitime();
                    
                    results.settime(m,z,((endtime-starttime)/2));
                    
                    buffer.checkBuffer(&everythingcorrect_check);
                    //buffer1.freeBuffer();
                }//else if
        }
        
        cout<<"\n";
        
    }//for iterations to get statistic errors
    
    /*------------------------------------ output-------------------------------*/
    MPI_Barrier(MPI_COMM_WORLD);
    Printoutput out;
    
    // send everything to process 0 to do the output
    if (rank == 1){
    
        //results.results();
     
        //mpi1.performsend(&recv_mean,numberofpackages,MPI_DOUBLE,0,numberofpackages+1,MPI_COMM_WORLD, sendmode);*/
    }
    else if (rank == 0) {

        if(everythingcorrect_check==0){
            
            //get information from process 1
            //double recv_mean[numberofpackages];
            //mpi1.performrecv(recv_mean,numberofpackages,MPI_DOUBLE,1,numberofpackages+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            //compare it!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            
            //--------------results all needed parameters & print them------------------
        
            // Header
            out.printtimestemp();
            out.printheader();
            
            results.calculate();
        }//if everything correct
        
        /*else{
            out.printerrormessage(everythingcorrect_check,p);
        }*/
    }//if you are process 0
#endif
    
    mpi1pnter->endmpi();
}