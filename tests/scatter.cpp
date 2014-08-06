#include <string>
#include <iostream>
#include <mpi.h>
#include <queue>
#include <unistd.h>
#include "tsc.h"

using namespace std;
/* 01.08.2014 gcc scatter.cpp -o scatter -Wall -std=c++11 -lstdc++ -I/opt/openmpi/1.6.5/include -pthread -L/opt/openmpi/1.6.5/lib -lmpi_cxx -lmpi -ldl -lm -lnuma -Wl,--export-dynamic -lrt -lnsl -lutil -lm -ldl (mpic++ -showme)*/

//The function we want to make the thread run.

void task1(string msg, int rank)
{
    cout << "task1 says: " << msg << " " << rank << endl;
}


int main(int argc, char *argv[]){
    int rank,length,size;
    char name[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);
    
    cout << "# process " << rank << " on host " << name << " reports for duty" << endl;
    
    //size_t innerRuntimeIterations = 1;
    unsigned int outerstatisticaliteration = 10;
    //MPI_Request comm_obj;
    //size_t index;
    double starttime, endtime;
    size_t buffersize = 1<<23;
    int *sendbuffer = new int [buffersize/sizeof(int)];
    int *recvbuffer = new int [buffersize/sizeof(int)];
    
    for (size_t i = 0; i < (buffersize / sizeof(int)); i++){
        sendbuffer[i] = rank;
    }
    
    //index = packagecount*((j*numberofReceivers)+(remoterank -1))%(buffersize/sizeof(int));
    
    for (unsigned int m = 0; m < outerstatisticaliteration; m++){
        for (size_t packagecount = 1; packagecount < 1<<20 ; packagecount = packagecount * 2){
            starttime = MPI_Wtime();
            for (int i = 0; i < size; i++){
                MPI_Scatter(sendbuffer, packagecount, MPI_INT,
                            recvbuffer, packagecount, MPI_INT,
                            i, MPI_COMM_WORLD);
                MPI_Barrier(MPI_COMM_WORLD);
            }
            endtime = MPI_Wtime();
            if (rank == 0){
                cout << packagecount*(size-1) << " - " << packagecount << " " << endtime - starttime << " - " << (packagecount*(size-1))/((endtime-starttime)*1000000) << " - " << rank << endl;
            }
            
            MPI_Barrier(MPI_COMM_WORLD);
        }//packagecount
    }//m
    
    
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(3);
    cout << "finished" << endl;
    
    
    MPI_Finalize();
    return 0;
}