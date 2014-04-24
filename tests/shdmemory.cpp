#include <mpi.h>
#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>

using boost::interprocess;
using namespace std;

int main(int argc,char *argv[]){
    int size, rank, length;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    cout << "hello this is process " << rank << endl;
    
    MPI_Finalize();
    return 0;
    
}
