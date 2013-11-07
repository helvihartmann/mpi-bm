#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <time.h>
//#include <-lrt>
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
    
    // read in data to send from console (default =1MB)
    if (argc > 1){
        ncounts = atoll(argv[1]);
    }
    else{
        ncounts = 1000000;
    }
    int nelem=3;
    double loadavg[nelem];
    
    systemload = getloadavg(loadavg, nelem);
    printf("last 15 minutes %f\n",loadavg[2]);
    printf("last 5 minutes %f\n",loadavg[1]);
    printf("last minute %f\n",loadavg[0]);
    
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
    //printf("%f",clock_gettime(CLOCK_PROCESS_CPUTIME_ID));
    clockid_t clk_id = CLOCK_REALTIME;
    struct timespec tp;
    double time =clock_gettime(clk_id,&tp);
    printf("%f",time);
}
