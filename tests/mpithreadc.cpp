#include <string>
#include <iostream>
#include <thread>
#include <mpi.h>

using namespace std;
/* gcc mpithreadc.cpp -o mpithreadc -Wall -std=c++11 -lstdc++ -I/opt/openmpi/1.6.5/include -pthread -L/opt/openmpi/1.6.5/lib -lmpi_cxx -lmpi -ldl -lm -lnuma -Wl,--export-dynamic -lrt -lnsl -lutil -lm -ldl (mpic++ -showme)*/

//The function we want to make the thread run.

void task1(string msg, int rank)
{
    cout << "task1 says: " << msg << " " << rank << endl;
}

class test {

public:
    void checkbuffer(int *buffer){
        if (buffer[0] == 14){
        cout << "super" << endl;
        }
        else{
            cout << "tatum " << buffer[0] << endl;
        }
    }

};

int main(int argc, char *argv[])
{
    int provided,rank;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cout << "I am rank " << rank << endl;
    
    int *buffer = new int [1];
    
    test my_test;
    
    // Constructs the new thread and runs it. Does not block execution.
    if (rank == 0){
        thread t1(task1, "Hello", rank);
        buffer[0] = 14;
        MPI_Send(buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        t1.join();
    }
    else {
        thread t1(task1, "kaese", rank);
        my_test.checkbuffer(buffer);
        buffer[0] = 1;
        
        MPI_Recv(buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << *buffer << endl;
        thread t2(my_test.checkbuffer, buffer);
        t1.join();
        //t2.join();
    }
    
    task1("end", rank);
    MPI_Finalize();
    return 0;
}
