#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <unistd.h>
#include "classmpi3.h"
#include "bandwidthcalc.h"
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
    
    //sleep(15);
    
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
    
    //cout << "# sendmode: " << *sendmode << " start Packagesize: "<<startPackageSize << " cutoff " << cutoff<<"\n";
    
    int outerStatisticalIterations = data.getstatisticaliterations();
    int numberofpackages = log(cutoff)/log(2)-log(startPackageSize)/log(2)+1;
    double starttime_send, endtime_send, starttime_recv, endtime_recv;
    double recvtime[outerStatisticalIterations][numberofpackages];
    double sendtime[outerStatisticalIterations][numberofpackages];
    double recv_summe[outerStatisticalIterations];
    
    for(int m=0; m<outerStatisticalIterations; m++){
        
        int z =0;
        size_t *everythingcorrect_check = 0;
        for(size_t p=startPackageSize; p<cutoff;p=p*2){
        
            
            /* --------------send/recv the data*-----------------------------------------*/
            data.setPackagesizeTmp(p);//p correct at this point
            size_t innerRuntimeIterations = data.getinnerRuntimeIterations(z);
            
            /*----------------------repeadingly send the package---------------------*/
                
                //Process 0 sends the data
                if (rank == 0) {
                    
                    Bufferoperations bufferop0(p, innerRuntimeIterations, sendmode, mpi1pnter);
                    bufferop0.allocateBuffer();
                    bufferop0.initalizeBuffer();
                    
                    // time measure sending process
                    starttime_send = mpi1.get_mpitime();
                    bufferop0.sendBuffer();//Objekt mpi1 mitübergeben
                    endtime_send = mpi1.get_mpitime();
                    sendtime[m][z]=(endtime_send-starttime_send);
                    
                    bufferop0.freeBuffer();
                 }
                
                //Process 1 receives the data
                else if (rank == 1) {
                    
                    Bufferoperations bufferop1(p, innerRuntimeIterations, sendmode, mpi1pnter);
                    bufferop1.allocateBuffer();
                    
                    //time measure receving data
                    starttime_recv = mpi1.get_mpitime();
                    bufferop1.recvBuffer();
                    endtime_recv = mpi1.get_mpitime();
                    
                    recvtime[m][z]=(endtime_recv-starttime_recv);
                    recv_summe[z]+=recvtime[m][z];
                    //std::cout<<recvtime[m][z]<<" ";
                    
                    bufferop1.checkBuffer(everythingcorrect_check);
                    bufferop1.freeBuffer();
                }//else if
            z++;
        }//for p iteration over package size
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
                recv_mean[z]=recv_summe[z]/outerStatisticalIterations;
                
                for (int m=0;m<outerStatisticalIterations;m++){
                    diff[z]+= pow((recv_mean[z] - recvtime[m][z]),2);
                }
                
                recv_var[z] = sqrt(diff[z])/outerStatisticalIterations;
                cout<<recv_mean[z]<<" ";
                cout<<recv_var[z]<<"\n";
                
            }
            
         
         //mpi1.performsend(recvtime,outerStatisticalIterations,MPI_DOUBLE,0,innerRuntimeIterations+1,MPI_COMM_WORLD, sendmode);
        }
        /*else if (rank == 0) {

            if(everythingcorrect_check==0){
                //get information from process 1
                mpi1.performrecv(recvtime,outerStatisticalIterations,MPI_DOUBLE,1,innerRuntimeIterations+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                
                //--------------calculate all needed parameters---------------------
                size_t totaldatasent = data.getTotalDataSent();

                
                Bandwidthcalc send(sendtime,outerStatisticalIterations);
                
                double send_mean = send.getmean();
                long double send_rate = send.getrate(totaldatasent);
                double send_var = send.getvar();
                
         
                // systemload
                int nelem=3;
                double loadavg[nelem];
                //int systemload = getloadavg(loadavg, nelem);
                getloadavg(loadavg, nelem);

                
                //-----------------------------print-- ------------------------------
                
                // Header
                if(p==startPackageSize){
                    out.printtimestemp();
                    out.printheader();
                }
                
                out.printbandwidth(innerRuntimeIterations, p, send_mean, send_rate, send_var, receive_rate, recv_var, loadavg);
        
            }//if everything correct
            
            else{
                out.printerrormessage(everythingcorrect_check,p);
            }
        }//if you are process 0*/
        

    
        
    mpi1.endmpi();
}