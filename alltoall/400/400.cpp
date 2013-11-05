#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <fstream>
#include <string>
//#include "classmpi.h"
#include "classmpi1.h"
#include <iostream>
using namespace std;

/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 07.10.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes
 
! This program shows how to use MPI_Alltoall.  Each processor
! send/rec a different created number*/

//#define mpi_root 0

int main(int argc,char *argv[]){
	int *sray,*rray;
	int *scounts,*rcounts;
    int size, rank, nsize, ncounts;
	int ssize,rsize,i,j,m, k;
	float z;
    
    // read in data to send from console (default =1)
    if (argc > 1){
        ncounts = atoi(argv[1]);
    }
    else{
        ncounts = 1;
    }
    
// start MPI
    Mpi mpi1;
	mpi1.init_it(&argc,&argv);
    size = mpi1.get_size();
    rank = mpi1.get_rank();
    
    nsize = size*ncounts;
    
//Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
    
	scounts= new int [nsize];
	rcounts= new int [nsize];
    printf("nsize: %d, sizeof array: %ld",nsize, sizeof(scounts));
    //printf("This is process %d with: scounts=",rank);
    for(i=0; i<nsize; i++){
        scounts[i]=rank;
        //printf("%d ",scounts[i]);
    }
    printf("\n");

/* send the data*/
    mpi1.performalltoall(scounts, ncounts,MPI_INT,
                         rcounts,ncounts,MPI_INT,
                         MPI_COMM_WORLD);
    
    printf("\nThis is process %d checking for correct data transmisson \n",rank);
    m=-1;
    int everythingcorrect_check = 0;
	for(i=-1;i<(nsize-ncounts);){//nsize-ncounts weil i in der k schleife noch um ncounts hochgezählt wird
        m++;
        for(k=0; k<ncounts; k++){
            i++;
            //printf("i = %d; m = %d: ;rcounts[i]:%d ",i, m, rcounts[i]);
            if(rcounts[i]==m){
                //printf("correct \n");
                everythingcorrect_check = 1;
            }
            else{
                printf("error at position %d \n",i);
            }
        }
    }
	printf("\n");
    
    if(everythingcorrect_check==1){
        printf("process %d found everything correct \n",rank);
    }
    
    
/* write data into output
    fstream Ausgabe;
    string name=argv[0];
    name +=".out";
    name +=rank;// warum ? statt zahl
    
	Ausgabe.open(name.c_str(), ios::out|ios::binary);
    Ausgabe<<"rank="  <<rank<<"\n"<<"rcounts=\n ";
	for(j=0;j<(nsize);j++)
        Ausgabe<<rcounts[j]<<" ";
        Ausgabe<<"\n";
    Ausgabe.close();*/
    
// end everything
    mpi1.endmpi();
    delete [] scounts;
    delete [] rcounts;
}