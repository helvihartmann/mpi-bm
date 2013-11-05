// headerdatei that defines MPI Functions as a class
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/classmpi.cpp
 12.10.2013
 additional global Variables are replaced by a class Mpi*/
#include "classmpi.h"


void Mpi::init_it(int  *argc, char ***argv) {
	mpi_err = MPI_Init(argc,argv);
}

int Mpi::get_size(){
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    return size;
}

int Mpi::get_rank(){
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return rank;
}


