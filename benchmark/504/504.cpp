#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <fstream>
#include <string>
#include "classmpi4.h"
#include <iostream>
#include <cstdint>
#include "cputime1.h"
#include <time.h>
using namespace std;

/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 07.11.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes even more than 1GB
 
! This program shows how to use MPI_Alltoall.  Each processor
! send/rec NONBLOCKING a different created number*/

//#define mpi_root 0
void timestemp();

int main(int argc,char *argv[]){
    int iterations = 10;
    int iterations2;
    int *scounts,*rcounts;
    size_t ncounts;
    int size, rank;
    float z;
    double starttime_send, endtime_send, starttime_recv, endtime_recv;
    int length;
    char name[MPI_MAX_PROCESSOR_NAME];
    double recvtime[iterations], sendtime[iterations];
    double startTime, endTime;
    MPI_Request requestout, requestin;
    
    // read in data to send from console (default =1MB)
    if (argc > 1){
        ncounts = atoll(argv[1]);
        iterations2 = atoll(argv[2]);
    }
    else{
        ncounts = 128;
        iterations2 = 1000000;
    }
    
// start MPI
    Mpi mpi1;
	mpi1.init_it(&argc,&argv);
    size = mpi1.get_size();
    rank = mpi1.get_rank();
    MPI_Get_processor_name(name, &length); //not yet handled in class
    
//Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
	scounts= new int [ncounts];
	rcounts= new int [ncounts];

/* send/recv the data*/
    
    int everythingcorrect_check = 0;
    size_t l = 0;
    
    for(size_t m=1; m<iterations; m++){
        
        //Process 0 sends the data
        if (rank == 0) {
            
            // initalize sendbuffer with zeroes
            for(size_t i=0; i<ncounts; i++){
                scounts[i]=rank;
            }
            
            //Information about sent data amount
            if(m==1){
                timestemp();
                printf("%ld Byte are send %d times\n",ncounts*sizeof(int),iterations2);
            }
            
            // time measure sending process
            startTime = getCPUTime( );
            starttime_send = mpi1.get_mpitime();
            for(int j=0; j<iterations2; j++){
                mpi1.performIsend(scounts,ncounts,MPI_INT,1,j,MPI_COMM_WORLD);
                //!!!!!! DRY!!!!!!!!!!!!!
                requestout = mpi1.get_request();
                requestin = requestout;
                mpi1.completenonblocking(&requestin);
            }
            
            
            endtime_send = mpi1.get_mpitime();
            sendtime[m]=(endtime_send-starttime_send);
        }
        
        //Process 1 receives the data
        else if (rank == 1) {
            
            //time measure receving data
            starttime_recv = mpi1.get_mpitime();
            for(int j=0; j<iterations2; j++){
                mpi1.performIrecv(rcounts,ncounts,MPI_INT,0,j,MPI_COMM_WORLD);
                //!!!!!! DRY!!!!!!!!!!!!!
                requestout = mpi1.get_request();
                requestin = requestout;
                mpi1.completenonblocking(&requestin);
            }
            endtime_recv = mpi1.get_mpitime();
            recvtime[m]=(endtime_recv-starttime_recv);
           
            
            //Checking for correct data Transmission
            for(size_t i=0;i<ncounts;i++){
                if(rcounts[m]==0){
                    //printf("correct \n");
                    everythingcorrect_check = 1;
                }
                else{
                    l++;
                }
            }//for i
        }//else if

    }
    
    // Finalize MPI
    mpi1.endmpi();
    delete [] scounts;
    delete [] rcounts;
    
    endTime = getCPUTime( );
    
    //print output
    // first process 0
    if (rank == 0) {
        
        int nelem=3;
        double loadavg[nelem];
        
        startTime = getCPUTime( );
        int systemload = getloadavg(loadavg, nelem);
        printf("%f load average last 15 minutes \n",loadavg[2]);
        printf("%f load average last 5 minutes\n",loadavg[1]);
        printf("%f load average last minute\n",loadavg[0]);
        
        //fprintf( stderr, "CPU time used = %lf\n", (endTime - startTime) );
        printf("CPU time used = %lf\n", (endTime - startTime) );

        
        for(size_t m=1; m<iterations; m++){
            double s = sendtime[m];
            printf("%f seconds for Process %d to send.\n",s ,rank);
        }
    }
    
    //now process 1
    if(rank==1){
        for(size_t m=1; m<iterations; m++){
            double r = recvtime[m];
            printf("%f seconds for Process %d to receive.\n", r,rank);
        }
        printf("\n");
        
        // Error Message
        if(everythingcorrect_check==1){
            printf("everything send correctly \n");
        }
        else{
            printf("%ld errors were spotted\n",l);
        }
    }
}

void timestemp(){
    time_t Zeitstempel;
    tm *nun;
    Zeitstempel = time(0);
    nun = localtime(&Zeitstempel);
    cout << nun->tm_mday << '.' << nun->tm_mon+1 << '.'
    << nun->tm_year+1900 << " - " << nun->tm_hour
    << ':' << nun->tm_min << endl;
    }