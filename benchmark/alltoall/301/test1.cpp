#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 07.10.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes
 
! This program shows how to use MPI_Alltoall.  Each processor
! send/rec a different created number*/

//#define mpi_root 0

int main(int argc,char *argv[]){
    //fstream Ausgabe;
    int k,i,j,m;
    int* rcounts;
	  
    int size =4;
    int ncounts = 3;
    int nsize = size*ncounts;
    
    rcounts= new int [nsize];
    for(i=0; i<nsize; i++){
        rcounts[i]=1;
    }
    for (i=0;i<nsize;){
        for(k=0; k<ncounts; k++){
            i++;
            cout<<k<<" "<<i<<"\n";
        }
        cout <<"\n";
    }

    delete [] rcounts;

}