#include <string>
#include <iostream>
#include <mpi.h>
#include <queue>
#include "tsc.h"

using namespace std;
/* gcc mpithreadc.cpp -o mpithreadc -Wall -std=c++11 -lstdc++ -I/opt/openmpi/1.6.5/include -pthread -L/opt/openmpi/1.6.5/lib -lmpi_cxx -lmpi -ldl -lm -lnuma -Wl,--export-dynamic -lrt -lnsl -lutil -lm -ldl (mpic++ -showme)*/

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
    
    size_t buffersize = 1<<20;
    int *buffer = new int [buffersize/sizeof(int)];
    
    for (size_t i = 0; i < (buffersize / sizeof(int)); i++){
        buffer[i] = rank;
    }

    
    size_t innerRuntimeIterations = 1000;
    size_t packagecount = 16384;
    unsigned int pipelinedepth = 10;
    MPI_Request comm_obj;
    size_t index;
    
    
    unsigned int numberofReceivers = 2;
    //warmup
    if (rank == 0){
        for (size_t j = 0; j < innerRuntimeIterations; j++){
            for(unsigned int remoterank = 1; remoterank <= numberofReceivers; remoterank++){
                index = packagecount*((j*numberofReceivers)+(remoterank -1))%(buffersize/sizeof(int));
                MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
                MPI_Wait (&comm_obj, MPI_STATUS_IGNORE);
            }
        }
        
    }
    else{
        for (size_t j = 0; j < innerRuntimeIterations; j++){
            index = (packagecount*j)%(buffersize/sizeof(int));
            MPI_Irecv((buffer + index), packagecount, MPI_INT, 0, 1, MPI_COMM_WORLD, &comm_obj);
            MPI_Wait (&comm_obj, MPI_STATUS_IGNORE);
        }
    }
    // Constructs the new thread and runs it. Does not block execution.
    
    
    innerRuntimeIterations = 1000000;
    std::vector<unsigned long long>cycles_comm;
    std::vector<size_t>testwaitcounter;
    
    if (rank == 0){
        std::vector<queue<MPI_Request>> vec(numberofReceivers);
        // flags to keep track of status of objects
        int flag = 0;
        std::vector<size_t>sendcount (numberofReceivers,0);
        size_t sendcountsum = 0;
        unsigned int index_receiver = -1;

        
        MPI_Barrier(MPI_COMM_WORLD);
        //filling pipes
        for(size_t j = 0; j < pipelinedepth; j++){
            for(unsigned int remoterank = 1; remoterank <= numberofReceivers; remoterank++){
                index_receiver = remoterank - 1;
                index = packagecount*((j*numberofReceivers)+index_receiver)%(buffersize/sizeof(int)); //remoterank-1 = index_receiver
                MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
                vec[index_receiver].push(comm_obj);
            }
        }
        // queue management ----------------------------
        
        while (sendcountsum != innerRuntimeIterations) {
            for(unsigned int remoterank = 1; remoterank <= numberofReceivers; remoterank++){
                unsigned int index_receiver = remoterank - 1;
            //dont account for receiver if has send already all packages, count how much he will send
                MPI_Test(&vec[index_receiver].front(), &flag, MPI_STATUS_IGNORE);
                //cout << rank << " checking " << remoterank << " flag is " << flag << endl;
                if (flag == 1){
                    vec[index_receiver].pop();
                    index = packagecount*((sendcount.at(index_receiver)*numberofReceivers)+index_receiver)%(buffersize/sizeof(int));
                    MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
                    sendcount.at(index_receiver)++;
                    sendcountsum++;
                    //cout << rank << " " << sendcount.at(index_receiver) << " to " << remoterank << " " << sendcountsum << endl;
                    vec[index_receiver].push(comm_obj);
                }
            }
        }
        cout << sendcount.at(0) << " " << sendcount.at(1) << endl;
        MPI_Finalize();
        return 0;

    }
    else{
        std::queue<MPI_Request> queue_request;
        // flags
        int flag = 0;
        size_t index;
        size_t recvcount = 0;

        MPI_Barrier(MPI_COMM_WORLD);
        // fill queue-----------------------------------
        for(size_t j = 0; j < pipelinedepth; j++){
                index = (packagecount*j)%(buffersize/sizeof(int));
                MPI_Irecv((buffer + index), packagecount, MPI_INT, 0, 1, MPI_COMM_WORLD, &comm_obj);
                queue_request.push(comm_obj);
        }
        // queue management-----------------------------
        while (recvcount != innerRuntimeIterations) {
            MPI_Test(&queue_request.front(), &flag, MPI_STATUS_IGNORE);
            //cout << rank << " test flag = " << flag << " " << recvcount << " " << innerRuntimeIterations << endl;
            if (flag == 1){
                queue_request.pop();
                index = (packagecount*recvcount)%(buffersize/sizeof(int));
                MPI_Irecv((buffer + index), packagecount, MPI_INT, 0, 1, MPI_COMM_WORLD, &comm_obj);
                recvcount++;
                //cout << rank << " " << recvcount << endl;
                queue_request.push(comm_obj);
            }

        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}