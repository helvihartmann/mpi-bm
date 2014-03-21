#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <queue>
using namespace std;

/* fles:~/benchmark/sendrecv 30.01.14
 this is the main.cpp (pingpong.cpp) to perform a MPI benchmark where process 0 sends data to process 1 and gets it back, for which the time is meassured in order to calculate the data rate*/

int main(int argc,char *argv[]){
    
    /*--------------------------------------start MPI-----------------------------*/
    int size, rank, length;
    char name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &length);
    
    size_t buffersize=pow(2,15);
    int* buffer = new int [buffersize / sizeof(int)];
    size_t maxpackage = 4*sizeof(int);
    int counter = 0;
    std::queue<MPI_Status> queue_status;
    std::queue<MPI_Request> queue_request;
    int numberofcalls=4;

    /*--------------------- read in parameters-----------------------------*/
    
    for (int p = 4; p < maxpackage; p=p*2){
        
        size_t packageCount = p/sizeof(int);
        size_t innerRuntimeIterations = 10;
        counter++;
        //Process 0 sends the data and gets it back
        if (rank == 0) {
            
            for (size_t i=0; i<buffersize / sizeof(int); i++){
                buffer[i]=counter;
            }
            
            for(size_t j=1; j<innerRuntimeIterations; j++){
                MPI_Request send_obj;
                MPI_Status status;
                if (queue_request.size() >= numberofcalls){
                    MPI_Wait (&queue_request.front(), &queue_status.front());
                    queue_status.pop();
                    queue_request.pop();
                }
                MPI_Isend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, 1, j, MPI_COMM_WORLD, &send_obj);
                queue_status.push(status);
                queue_request.push (send_obj);
            }
            while (!queue_request.empty()){
                MPI_Wait (&queue_request.front(), &queue_status.front());
                queue_request.pop();
                queue_status.pop();
            }
        }
        
            
        //Process 1 receives the data and sends it back
        else if (rank == 1) {
            
            for(size_t j=1; j<innerRuntimeIterations; j++){
                MPI_Request recv_obj;
                MPI_Status status;
                if (queue_request.size() >= numberofcalls){
                    MPI_Wait (&queue_request.front(), &queue_status.front());
                    queue_status.pop();
                    queue_request.pop();
                }
                
                MPI_Isend((buffer + ((packageCount*j)%buffersize)), packageCount, MPI_INT, 0, j, MPI_COMM_WORLD, &recv_obj);
                queue_status.push(status);
                queue_request.push(recv_obj);
            }
            while (!queue_request.empty()){
                MPI_Wait (&queue_request.front(), &queue_status.front());
                queue_request.pop();
                queue_status.pop();
            }
        }
    }
    MPI_Finalize();
}