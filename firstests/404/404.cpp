#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <fstream>
#include <string>
#include "classmpi2.h"
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
    int *scounts,*rcounts;
    //int size, rank;
    // uint64_t nsize;
    size_t nsize, ncounts;
    int size, rank;
    //size_t i,j,m, k,l ;
    float z;
    double starttime, endtime;
    int length;
    char name[MPI_MAX_PROCESSOR_NAME];
    
    // read in data to send from console (default =1)
    if (argc > 1){
        ncounts = atoll(argv[1]);
    }
    else{
        ncounts = 1;
    }
    
// start MPI
    Mpi mpi1;
	mpi1.init_it(&argc,&argv);
    size = mpi1.get_size();
    rank = mpi1.get_rank();
    MPI_Get_processor_name(name, &length); //not yet handled in class
    nsize = size*ncounts;
    
//Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
    
	scounts= new int [nsize];
	rcounts= new int [nsize];
    printf("process %d: %ldkB of memory are allocated", rank,(nsize*2)/1000);
    //printf("This is process %d with: scounts=",rank);
    for(size_t i=0; i<nsize; i++){
        scounts[i]=rank;
        //printf("%d ",scounts[i]);
    }
    //printf("\n");

/* send the data*/
    starttime = mpi1.get_mpitime();
    mpi1.performalltoall(scounts, ncounts,MPI_INT,
                         rcounts,ncounts,MPI_INT,
                         MPI_COMM_WORLD);
    endtime = mpi1.get_mpitime();
    
    printf("\n process %d on %s checking for correct data transmisson \n",rank,name);
    size_t m=-1;
    size_t l=0;
    int everythingcorrect_check = 0;
    
	for(size_t i=0;i<(nsize-ncounts);){//nsize-ncounts weil i in der k schleife noch um ncounts hochgezählt wird
        m++;
        for(size_t k=0; k<ncounts; k++){
            //printf("i = %d; m = %d: ;rcounts[i]:%d ",i, m, rcounts[i]);
            if(rcounts[i]==m){
                //printf("correct \n");
                everythingcorrect_check = 1;
            }
            else{
                l++;
                //printf("error at position %d \n",i);
                //printf("%d; ",i);
            }
            i++;
        }
    }
	//printf("\n");
    
    if(everythingcorrect_check==1){
        printf("process %d running found everything correct \n",rank);
    }
    else{
        printf("%ld errors were spotted",l);
    }
    printf("%f seconds for Process %d.\n", (endtime-starttime),rank);
    
    mpi1.endmpi();
    delete [] scounts;
    delete [] rcounts;
}
