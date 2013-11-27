#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
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
    Mpi mpi1;
    mpi1.init_it(&argc,&argv);
    size = mpi1.get_size();
    rank = mpi1.get_rank();
    MPI_Get_processor_name(name, &length); //not yet handled in class
    cout<<"# Prozess " << rank << " von " <<size<<" on "<< name<<" \n";

    
    /*--------------------- Iterate over packege size-----------------------------*/
    //get starting packege size; read in data to send from console (default =128B)
    Totaldatasendcalc data;
    
    size_t ncounts = data.getpackagesize(argc,argv);
    size_t cutoff = data.getcutoff(argc, argv);
    
    for(size_t p=ncounts; p<cutoff;p=p*2){
        int m=0;
        //cout<<"jez mache ich das schon zum" << m++ <<". mal;   " << p << " = Packetgröße;  " << "Prozess" << rank << " von" <<size<<" on"<< name<<" \n";
        int iterations = 10;
        double starttime_send, endtime_send, starttime_recv, endtime_recv;
        double recvtime[iterations], sendtime[iterations];
        size_t *everythingcorrect_check = 0;
        /* --------------send/recv the data*-----------------------------------------*/
        Bufferoperations bufferoperations;
        bufferoperations.setPackagesizeTmp(p);
        bufferoperations.allocateBuffer();
        
        data.setPackagesizeTmp(p);
        size_t iterations2 = data.getiterations2();
        //cout<<"ITERATIONS2  "<<iterations2<<"\n";
        /*----------------------repeadingly send the package---------------------*/
        for(int m=0; m<iterations; m++){
            
            //Process 0 sends the data
            if (rank == 0) {

                bufferoperations.initalizeBuffer();

                // time measure sending process
                starttime_send = mpi1.get_mpitime();
                for(int j=0; j<iterations2; j++){
                    bufferoperations.sendBuffer();
                }
                endtime_send = mpi1.get_mpitime();
                sendtime[m]=(endtime_send-starttime_send);
             }
            
            //Process 1 receives the data
            else if (rank == 1) {
                
                //time measure receving data
                starttime_recv = mpi1.get_mpitime();
                for(int j=0; j<iterations2; j++){
                    bufferoperations.recvBuffer();
                }
                endtime_recv = mpi1.get_mpitime();
                recvtime[m]=(endtime_recv-starttime_recv);
                
                bufferoperations.checkBuffer(everythingcorrect_check);
            }//else if
        }//for iterations to get statistic errors
        /*------------------------------------ output-------------------------------*/
        MPI_Barrier(MPI_COMM_WORLD);
        Printoutput out;
        
        // send everything to process 0 to do the output
        if (rank == 1){
            mpi1.performsend(recvtime,iterations,MPI_DOUBLE,0,iterations2+1,MPI_COMM_WORLD);
        }
        if (rank == 0) {

            if(everythingcorrect_check==0){
                //get information from process 1
                mpi1.performrecv(recvtime,iterations,MPI_DOUBLE,1,iterations2+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                
                /*--------------calculate all needed parameters--------------------- */
                size_t totaldatasent = data.getTotalDataSent();

                Bandwidthcalc calc;
                double send_mean = calc.getmean(sendtime, iterations);
                double recv_mean = calc.getmean(recvtime, iterations);
                long double send_rate = calc.getrate(send_mean,iterations2, p, totaldatasent);
                long double receive_rate = calc.getrate(recv_mean,iterations2, p, totaldatasent);
                double send_var = calc.getvar(sendtime, iterations, send_mean);
                double recv_var = calc.getvar(recvtime, iterations, send_mean);

                
                // systemload
                int nelem=3;
                double loadavg[nelem];
                int systemload = getloadavg(loadavg, nelem);
                
                /*-----------------------------print-- ------------------------------*/
                
                // Header
                if(p==ncounts){
                    out.printtimestemp();
                    out.printheader();
                }
                
                out.printbandwidth(iterations2, p, send_mean, send_rate, send_var, receive_rate, recv_var, loadavg);
            }//if everything correct
            
            else{
                out.printerrormessage(everythingcorrect_check,p);
            }
        }//if you are process 0
        
        bufferoperations.freeBuffer();
    }//for p iteration over package size
        
    mpi1.endmpi();
}