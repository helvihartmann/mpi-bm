#include <string>
#include <iostream>
#include <mpi.h>


using namespace std;
/* mpic++ mpihellow.cpp -o mpihellow */

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
    
    MPI_Finalize();
    return 0;
}