#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <fstream>
#include <string>
#include "classmpi3.h"
#include <iostream>
#include <cstdint>
using namespace std;

/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 21.10.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes even more than 1GB
 
! This program shows how to use MPI_Alltoall.  Each processor
! send/rec a different created number*/

//#define mpi_root 0

int main(int argc,char *argv[]){
    int iterations = 10;
    //int *sray ,*rray;
    int *scounts,*rcounts;
    size_t ncounts, size, rank;
    //size_t j, k,l ;
    float z;
    double starttime_send, endtime_send, starttime_recv, endtime_recv;
    int length;
    char name[MPI_MAX_PROCESSOR_NAME];
    
    // read in data to send from console (default =1MB)
    if (argc > 1){
        ncounts = atoll(argv[1]);
    }
    else{
        ncounts = 1000000;
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
            
            //Information about sended data amount
            if(m==1){
                printf("%ld Byte are send\n",ncounts);
            }
            starttime_send = mpi1.get_mpitime();
            mpi1.performsend(scounts,ncounts,MPI_INT,1,0,MPI_COMM_WORLD);
            endtime_send = mpi1.get_mpitime();
            printf("%f seconds for Process %ld to send.\n", (endtime_send-starttime_send),rank);
        }
        
        //Process 1 receives the data
        else if (rank == 1) {
            starttime_recv = mpi1.get_mpitime();
            mpi1.performrecv(rcounts,ncounts,MPI_INT,0,0,
                             MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            endtime_recv = mpi1.get_mpitime();
            printf("%f seconds for Process %ld to receive.\n", (endtime_recv-starttime_recv),rank);
            
            //Checking for correct data Transmission
            for(size_t i=0;i<ncounts;i++){
                if(rcounts[i]==0){
                    //printf("correct \n");
                    everythingcorrect_check = 1;
                }
                else{
                    l++;
                }
            }//for i
        }//else if
    }
    
    // Error Message
    if(everythingcorrect_check==1){
        printf("everything send correctly \n");
    }
    else{
        printf("%ld errors were spotted\n",l);
    }

    // Finalize
    mpi1.endmpi();
    delete [] scounts;
    delete [] rcounts;
}
