#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include "classmpi3.h"
#include "print.h"
#include "totaldatasendcalc.h"
#include "bufferoperations.h"
#include "results.h"
#include <vector>
using namespace std;

/* fles:~/benchmark/sendrecv 27.11.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes as specified in console
 This version iterates over the packege size to assure for every run the same conditions on the fles
 and prints everything into sendrecv.out when runned via 
 > sbatch slurm_test.in*/

int main(int argc,char *argv[]){
    
    /*--------------------------------------start MPI-----------------------------*/
    //cout<<"\n hallo \n";
    int size, rank;
    int length;
    char name[MPI_MAX_PROCESSOR_NAME];
    Mpi mpi1;//Pointer auf dings anlegen und mpi1.sendbuffer(mpi1pnter) übergeben
    Mpi *mpi1pnter;
    mpi1pnter = &mpi1;
    
    mpi1.init_it(&argc,&argv);
    size = mpi1.get_size();
    rank = mpi1.get_rank();
    MPI_Get_processor_name(name, &length); //not yet handled in class
    cout<<"# Prozess " << rank << " von " <<size<<" on "<< name<<" \n";

    
    /*--------------------- Iterate over packege size-----------------------------*/
    //get starting packege size; read in data to send from console (default =128B)
    Totaldatasendcalc data;
    
    data.readOptions(argc,argv);
    size_t startPackageSize = data.getpackagesize();
    size_t cutoff = data.getcutoff();
    int tmp = data.getsendmode(); // 1 Send, 2 Ssend, 3 Bsend
    const int* sendmode = &tmp;
    
    int outerStatisticalIterations = data.getstatisticaliterations();
    int numberofpackages = log(cutoff)/log(2)-log(startPackageSize)/log(2)+1;
    
    double starttime, endtime;
 
    //-------Vector definations-------------------
    Results calculate(outerStatisticalIterations,numberofpackages);
    
    //double time[outerStatisticalIterations][numberofpackages];
    /*std::vector< vector<double> > time(outerStatisticalIterations, vector<double> (numberofpackages));*/
    

    /*std::vector<size_t>package_vector(numberofpackages);
    std::vector<size_t>innerRuntimeIterations_vector(numberofpackages);
    std::vector<size_t>totaldatasent_vector(numberofpackages);
    std::vector<double> loadavg_vector(numberofpackages);*/
    
    //double summe[numberofpackages];
    size_t *everythingcorrect_check = 0;
    
    
    for(int m=0; m<outerStatisticalIterations; m++){
        
        cout<<"# Statistical Iteration cycle "<<m<<"\n";
        
        Bufferoperations bufferop(sendmode, mpi1pnter,rank);
        
        size_t p(startPackageSize);
        
        //for(size_t p=startPackageSize; p<cutoff;p=p*2){
        for (int z=0; z < numberofpackages; z++){
            
            /* -------------- make first calculations on datavolume---------------------------*/
            data.setPackagesizeTmp(p);//p correct at this point
            size_t innerRuntimeIterations = data.getinnerRuntimeIterations(z);
            size_t totaldatasent = data.getTotalDataSent();
            
            
            /*----------------------repeadingly send the package---------------------*/
                
                //Process 0 sends the data
                if (rank == 0) {
                    
                    bufferop.setloopvariables(p, innerRuntimeIterations, 1);

                    calculate.setvectors(p, innerRuntimeIterations, totaldatasent,z);
                    //package_vector.at(z)=p;
                    //innerRuntimeIterations_vector.at(z)=innerRuntimeIterations;
                    //totaldatasent_vector.at(z)=totaldatasent;
                    
                    // time measure sending process
                    starttime = mpi1.get_mpitime();
                    bufferop.sendBuffer();
                    bufferop.recvBuffer();
                    endtime = mpi1.get_mpitime();
                    calculate.settime(m,z,((endtime-starttime)/2));
                    //time[m][z]=(endtime-starttime)/2;
                    
                    //systemload
                    int nelem=3;
                    double loadavg[nelem];
                    //int systemload = getloadavg(loadavg, nelem);
                    //getloadavg(loadavg, nelem);
                    //loadavg_vector.at(z)=loadavg[1];
                 }
                
                //Process 1 receives the data
                else if (rank == 1) {
                    
                    bufferop.setloopvariables(p, innerRuntimeIterations, 0);
                    
                    //time measure receving data
                    starttime = mpi1.get_mpitime();
                    bufferop.recvBuffer();
                    bufferop.sendBuffer();
                    endtime = mpi1.get_mpitime();
                    
                    calculate.settime(m,z,((endtime-starttime)/2));
                    /*time[m][z]=(endtime-starttime)/2;
                    summe.at(z)+=time[m][z];*/
                    
                    bufferop.checkBuffer(everythingcorrect_check);
                    //bufferop1.freeBuffer();
                }//else if
           p *= 2;
          //  z++;
        }//for p iteration over package size
        cout<<"\n";
        bufferop.freeBuffer();
    }//for iterations to get statistic errors

    /*------------------------------------ output-------------------------------*/
    MPI_Barrier(MPI_COMM_WORLD);
    Printoutput out;
    
    // send everything to process 0 to do the output
    if (rank == 1){
    
        //calculate.calculate();
     
        //mpi1.performsend(&recv_mean,numberofpackages,MPI_DOUBLE,0,numberofpackages+1,MPI_COMM_WORLD, sendmode);*/
    }
    else if (rank == 0) {

        if(everythingcorrect_check==0){
            
            //get information from process 1
            //double recv_mean[numberofpackages];
            //mpi1.performrecv(recv_mean,numberofpackages,MPI_DOUBLE,1,numberofpackages+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            //compare it!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            
            //--------------calculate all needed parameters & print them------------------
        
            // Header
            out.printtimestemp();
            out.printheader();
            
            calculate.calculate();
        }//if everything correct
        
        /*else{
            out.printerrormessage(everythingcorrect_check,p);
        }*/
    }//if you are process 0
        
    mpi1.endmpi();
}