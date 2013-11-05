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
 
! This program shows how to use MPI_Alltoall.  Each processor
! send/rec a different created number*/

//#define mpi_root 0

int main(int argc,char *argv[]){
	int *sray,*rray;
	int *scounts,*rcounts;
    int size, rank;
	int ssize,rsize,i,k,j;
	float z;
    fstream Ausgabe;
    Mpi mpi1;

	mpi1.init_it(&argc,&argv);
    size = mpi1.get_size();
    rank = mpi1.get_rank();
//Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
	scounts=(int*)malloc(sizeof(int)*size);
	rcounts=(int*)malloc(sizeof(int)*size);
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Änderung!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! rank of each process is multiplied with 100 and for each process a set of data from rank to rank*100+size is created
*/
	for(i=0;i<size;i++){
		//random_number(&z);
		scounts[i]=(int)(100.0*(rank+1))+i;
	}
	printf("This is process %d with: scounts=",rank);
	for(i=0;i<size;i++)
		printf("%d ",scounts[i]);
	printf("\n");
    //printf(("%s"),argv[0]); nur Test ob das auch ein String ist ist
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Änderung Ende!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/* send the data*/
    mpi1.performalltoall(scounts, 1,MPI_INT,
                         rcounts,1,MPI_INT,
                         MPI_COMM_WORLD);
    
    //for(k=0;k<<rank;k++)

    string name=argv[0];
    name +=".out";
    name +=rank;// warum ? statt zahl
    
	Ausgabe.open(name.c_str(), ios::out|ios::binary);
    Ausgabe<<"rank="  <<rank<<"rcounts= ";
	for(i=0;i<size;i++)
        Ausgabe<<rcounts[i]<<" ";
        Ausgabe<<"\n";
    Ausgabe.close();
    mpi1.endmpi();
    free (scounts);
    free (rcounts);
}