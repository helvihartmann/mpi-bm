#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example2.c
 07.10.2013
! This program shows how to use MPI_Alltoall.  Each processor
! send/rec a different created number 
*/
/* globals */
int size,rank,mpi_err;
#define mpi_root 0
/* end module  */

void init_it(int  *argc, char ***argv);
void init_it(int  *argc, char ***argv) {
	mpi_err = MPI_Init(argc,argv);
    mpi_err = MPI_Comm_size( MPI_COMM_WORLD, &size );
    mpi_err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}

int main(int argc,char *argv[]){
	int *sray,*rray;
	int *sdisp,*scounts,*rdisp,*rcounts;
	int ssize,rsize,i,k,j;
	float z;

	init_it(&argc,&argv);
//Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
	scounts=(int*)malloc(sizeof(int)*size);
	rcounts=(int*)malloc(sizeof(int)*size);
	sdisp=(int*)malloc(sizeof(int)*size);
	rdisp=(int*)malloc(sizeof(int)*size);
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
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Änderung Ende!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/* send the data */
	mpi_err = MPI_Alltoall(	scounts,1,MPI_INT,
						    rcounts,1,MPI_INT,
	                 	    MPI_COMM_WORLD);
	printf("rank= %d rcounts=",rank);
	for(i=0;i<size;i++)
		printf("%d ",rcounts[i]);
	printf("\n");
    mpi_err = MPI_Finalize();
}