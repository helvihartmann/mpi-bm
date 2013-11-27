#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include "cputime1.h"
//#include <time.h>
#include <mach/clock.h>
#include <mach/mach.h>
using namespace std;

/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 21.10.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes even more than 1GB
 
! This program shows how to use MPI_Alltoall.  Each processor
! send/rec a different created number*/

//#define mpi_root 0

int main(int argc,char *argv[]){
    int *scounts;
    //int *rcounts;
    //int size, rank;
    // uint64_t nsize;
    size_t ncounts;
    int systemload;
    double startTime, endTime;
    
    // read in data to send from console (default =1MB)
    if (argc > 1){
        ncounts = atoll(argv[1]);
    }
    else{
        ncounts = 1000000;
    }
    int nelem=3;
    double loadavg[nelem];
    
    startTime = getCPUTime( );
    systemload = getloadavg(loadavg, nelem);
    printf("%f load average last 15 minutes \n",loadavg[2]);
    printf("%f load average last 5 minutes\n",loadavg[1]);
    printf("%f load average last minute\n",loadavg[0]);
    
//Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
	scounts= new int [ncounts];
	//rcounts= new int [nsize];
    for(size_t i=0; i<ncounts; i++){
        scounts[i]=0;
    }
    
    printf("%ld Byte are allocated\n",ncounts);

    
//Finalize
    delete [] scounts;
    //delete [] rcounts;
    
    endTime = getCPUTime( );
    fprintf( stderr, "CPU time used = %lf\n", (endTime - startTime) );
    
}