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
    double time[outerStatisticalIterations][numberofpackages];
    
    //-------Vector definations-------------------
    std::vector<double> summe(numberofpackages);
    //size_t package_vector[numberofpackages];
    std::vector<size_t>package_vector(numberofpackages);
    //size_t innerRuntimeIterations_vector[numberofpackages];
    std::vector<size_t>innerRuntimeIterations_vector(numberofpackages);
    //size_t totaldatasent_vector[numberofpackages];
    std::vector<size_t>totaldatasent_vector(numberofpackages);
    //double loadavg_vector[numberofpackages];
    std::vector<double> loadavg_vector(numberofpackages);
    
    //double summe[numberofpackages];
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
            
            /*----------------------repeadingly send the package---------------------*/
                
                //Process 0 sends the data
                if (rank == 0) {
                    
                    bufferop.setloopvariables(p, innerRuntimeIterations, 1);

                    package_vector[z]=p;
                    innerRuntimeIterations_vector[z]=innerRuntimeIterations;
                    totaldatasent_vector[z]=totaldatasent;
                    
                    // time measure sending process
                    starttime = mpi1.get_mpitime();
                    bufferop.sendBuffer();//Objekt mpi1 mitübergeben
                    bufferop.recvBuffer();
                    endtime = mpi1.get_mpitime();
                    time[m][z]=(endtime-starttime)/2;
                    
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
                    
                    bufferop.setloopvariables(p, innerRuntimeIterations, 0);
                    
                    //time measure receving data
                    starttime = mpi1.get_mpitime();
                    bufferop.recvBuffer();
                    bufferop.sendBuffer();
                    endtime = mpi1.get_mpitime();
                    
                    time[m][z]=(endtime-starttime)/2;
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
        
            // Header
            out.printtimestemp();
            out.printheader();
            
            //bandwith calculations
            //double send_mean[numberofpackages];
            // std::vector<double> send_mean(numberofpackages)
            //double send_stdtime[numberofpackages];
            // std::vector<double> send_stdtime(numberofpackages)
            //double send_std[numberofpackages];
            // std::vector<double> send_std(numberofpackages)
            //double rate[numberofpackages];
            // std::vector<double> rate(numberofpackages)
            
            for(int z=0;z<numberofpackages;z++){
                double send_mean=summe[z]/outerStatisticalIterations;
                //cout<<"summe["<<z<<"]: "<<summe[z]<<endl;
                //cout<<"send_mean "<<send_mean<<endl;
                double diff = 0;
                for (int m=0;m<outerStatisticalIterations;m++){
                    diff+= (send_mean - time[m][z])*(send_mean - time[m][z]);
                }
                double rate =(totaldatasent_vector[z]/send_mean)/1000000;
                //cout << totaldatasent_vector[z]<< endl;
                double send_vartime = diff/outerStatisticalIterations;
                double send_stdtime = sqrt(send_vartime);
                double send_std=(send_stdtime/send_mean)*rate;
                
                printf("%ld %ld %ld %f %f %f %f - %f \n",totaldatasent_vector[z] ,innerRuntimeIterations_vector[z], package_vector[z] *sizeof(int),send_mean, send_stdtime, rate, send_std, loadavg_vector[z]);

            }
        }//if everything correct
        
        /*else{
            out.printerrormessage(everythingcorrect_check,p);
        }*/
    }//if you are process 0
        
    mpi1.endmpi();
}