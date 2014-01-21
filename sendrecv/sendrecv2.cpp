#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <unistd.h>
#include "classmpi3.h"
#include "print.h"
#include "totaldatasendcalc.h"
#include "bufferoperations.h"
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
    size_t package_vector[numberofpackages];
    size_t innerRuntimeIterations_vector[numberofpackages];
    size_t totaldatasent_vector[numberofpackages];
    double loadavg_vector[numberofpackages];
    
    double starttime, endtime;
    double time[outerStatisticalIterations][numberofpackages];
    double summe[numberofpackages];
    size_t *everythingcorrect_check = 0;
    
    

    for(int m=0; m<outerStatisticalIterations; m++){
        
        cout<<"# Statistical Iteration cycle "<<m<<"\n";
        
        int z =0;
        Bufferoperations bufferop(sendmode, mpi1pnter,rank);
        
        for(size_t p=startPackageSize; p<cutoff;p=p*2){
        
            /* -------------- make first calculations on datavolume-----------------------------------------*/
            data.setPackagesizeTmp(p);//p correct at this point
            size_t innerRuntimeIterations = data.getinnerRuntimeIterations(z);
            size_t totaldatasent = data.getTotalDataSent();
            
            //--------------------------initalize buffer----------------------------------------
            bufferop.setloopvariables(p, innerRuntimeIterations);
            
            /*----------------------repeadingly send the package---------------------*/
                
                //Process 0 sends the data
                if (rank == 0) {
                    
                    package_vector[z]=p;
                    innerRuntimeIterations_vector[z]=innerRuntimeIterations;
                    totaldatasent_vector[z]=totaldatasent;
                    
                    // time measure sending process
                    starttime = mpi1.get_mpitime();
                    bufferop.sendBuffer();//Objekt mpi1 mitübergeben
                    endtime = mpi1.get_mpitime();
                    time[m][z]=(endtime-starttime);
                    
                    cout<<time[m][z]<<" ";
                    
                    summe[z]+=time[m][z];
                    
                    //systemload
                    int nelem=3;
                    double loadavg[nelem];
                    //int systemload = getloadavg(loadavg, nelem);
                    getloadavg(loadavg, nelem);
                    loadavg_vector[z]=loadavg[1];
                 }
                
                //Process 1 receives the data
                else if (rank == 1) {
                    
                    //Bufferoperations bufferop1(p, innerRuntimeIterations, sendmode, mpi1pnter);
                    //bufferop1.allocateBuffer();
                    
                    //time measure receving data
                    starttime = mpi1.get_mpitime();
                    bufferop.recvBuffer();
                    endtime = mpi1.get_mpitime();
                    
                    time[m][z]=(endtime-starttime);
                    summe[z]+=time[m][z];
                    
                    bufferop.checkBuffer(everythingcorrect_check);
                    //bufferop1.freeBuffer();
                }//else if
            z++;
        }//for p iteration over package size
        cout<<"\n";
        bufferop.freeBuffer();
    }//for iterations to get statistic errors

    /*------------------------------------ output-------------------------------*/
    MPI_Barrier(MPI_COMM_WORLD);
    Printoutput out;
    
    // send everything to process 0 to do the output
    if (rank == 1){
     
     //Bandwidthcalc recv(recvtime, recv_summe, outerStatisticalIterations);
     //double recv_mean = recv.getmean();
     //long double receive_rate = recv.getrate(totaldatasent);
     //double recv_var = recv.getvar();
        double recv_mean[numberofpackages];
        double recv_var[numberofpackages];
        double diff[numberofpackages];
        for(int z=0;z<numberofpackages;z++){
            recv_mean[z]=summe[z]/outerStatisticalIterations;
            diff[z]=0;
            for (int m=0;m<outerStatisticalIterations;m++){
                diff[z]+= pow((recv_mean[z] - time[m][z]),2);
            }
            
            recv_var[z] = sqrt(diff[z])/outerStatisticalIterations;
            //cout<<recv_mean[z]<<" r ";
            //cout<<recv_var[z]<<"\n";
        }
     
     mpi1.performsend(recv_mean,numberofpackages,MPI_DOUBLE,0,numberofpackages+1,MPI_COMM_WORLD, sendmode);
    }
    else if (rank == 0) {

        if(everythingcorrect_check==0){
            
            //get information from process 1
            double recv_mean[numberofpackages];
            mpi1.performrecv(recv_mean,numberofpackages,MPI_DOUBLE,1,numberofpackages+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            //compare it!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            
            //--------------calculate all needed parameters & print them------------------
            
            /*Bandwidthcalc send(sendtime,outerStatisticalIterations);
            double send_mean = send.getmean();
            long double send_rate = send.getrate(totaldatasent);
            double send_var = send.getvar();*/
        
            // Header
            out.printtimestemp();
            out.printheader();
            
            //bandwith calculations
            double send_mean[numberofpackages];
            double send_stdtime[numberofpackages];
            double send_std[numberofpackages];
            double rate[numberofpackages];
            
            for(int z=0;z<numberofpackages;z++){
                send_mean[z]=summe[z]/outerStatisticalIterations;
                double diff = 0;
                for (int m=0;m<outerStatisticalIterations;m++){
                    diff+= (send_mean[z] - time[m][z])*(send_mean[z] - time[m][z]);
                }
                rate[z]=(totaldatasent_vector[z]/send_mean[z])/1000000;
                cout << totaldatasent_vector[z]<< endl;
                double send_vartime = diff/outerStatisticalIterations;
                send_stdtime[z] = sqrt(send_vartime);
                send_std[z]=(send_stdtime[z]/send_mean[z])*rate[z];
                
                out.printbandwidth(innerRuntimeIterations_vector[z], package_vector[z], send_mean[z], send_stdtime[z],rate[z], send_std[z], loadavg_vector[z]);
            }
        }//if everything correct
        
        /*else{
            out.printerrormessage(everythingcorrect_check,p);
        }*/
    }//if you are process 0
        
    mpi1.endmpi();
}