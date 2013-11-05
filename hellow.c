/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include <stdio.h>
#include "mpi.h"

int main( int argc, char *argv[] )
{
    int rank;
    int size;
    int length;
    char name[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Init( 0, 0 );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);//rank=number of Process
    MPI_Comm_size(MPI_COMM_WORLD, &size);//size=number of all processes
    MPI_Get_processor_name(name, &length); 
    printf( "Hello world this is process %d of %d from %s \n", rank, size,name);
    MPI_Finalize();
    return 0;
}

