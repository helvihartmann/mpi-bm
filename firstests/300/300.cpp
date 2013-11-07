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
	int ssize,rsize,i,j;
	float z;
    fstream Ausgabe;
    
    // read in data to send from console (default =1)
    if (argc > 1){
        ncounts = atoi(argv[1]);
    }
    else{
        ncounts = 1;
    }
    
    /*cout << "How much data should each process send? insert value (default=1): \n";
    cin >> int ncounts;
    cout <<ncounts;
    if (cin >> ncounts){
    cout << ncounts << "(ok)\n";
    }
     
    else{
    ncounts =1; // was muss ich tun damit er wieder zum anfang der while schleife springt???
    }*/
    
// start MPI
    Mpi mpi1;
	mpi1.init_it(&argc,&argv);
    size = mpi1.get_size();
    rank = mpi1.get_rank();
    
    nsize = size*ncounts;
    
//Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
	scounts= new int [nsize];
	rcounts= new int [nsize];
    for(i=0; i<nsize; i++){
        scounts[i]=rank;
        //cout<<scounts[i]<<"\n";
    }

/* send the data*/
    mpi1.performalltoall(scounts, ncounts,MPI_INT,
                         rcounts,ncounts,MPI_INT,
                         MPI_COMM_WORLD);
    
// write data into output
    string name=argv[0];
    name +=".out";
    name +=rank;// warum ? statt zahl
    
	Ausgabe.open(name.c_str(), ios::out|ios::binary);
    Ausgabe<<"rank="  <<rank<<"\n"<<"rcounts=\n ";
	for(j=0;j<(nsize);j++)
        Ausgabe<<rcounts[j]<<" ";
        Ausgabe<<"\n";
    Ausgabe.close();
    
// end everything
    mpi1.endmpi();
    delete [] scounts;
    delete [] rcounts;
}