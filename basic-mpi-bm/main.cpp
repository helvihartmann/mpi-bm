#include <mpi.h>
#include <queue>

using namespace std;

/*2015 Copyright Helvi Hartmann <hhartmann@fias.uni-frankfurt.de> */

MPI_Comm setgroups(unsigned int numbercommprocesses, int rank);


int main (int argc, char *argv[]){
//initiate MPI-----------------------------------------------------------------------------------------------
    int rank, size, length;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);
   
    double starttime, endtime;
    unsigned int remoteRank;
    int pipelinedepth = 8;
    std::queue<MPI_Request> queue_request;
    MPI_Request comm_obj;
    size_t innerRuntimeIterations = 10000;
    size_t buffersize = 17179869184;

    int *buffer = new int[(buffersize / sizeof(int))];
    
    std::cout << "# " << buffersize << " B allocated, initializing...\n" << std::endl;
    
    for (size_t i=0; i < (buffersize / sizeof(int)); i++){
        buffer[i]=0;
    }
    
    std::cout << "# buffer initialized.\n" << std::endl;

    for (unsigned int iteration = 0; iteration < 2; iteration++){
        for(unsigned int phase = 0; phase < (size/2); phase++){
            for (unsigned int packagesize = 4; packagesize < 4194304; packagesize = packagesize *2){
            size_t packagecount = packagesize/sizeof(int);
                MPI_Barrier(MPI_COMM_WORLD);
                starttime = MPI_Wtime();
                if (rank%2==0){
                    //cout << phase << " " << rank << " sending to " << remoteRank << " " << packagesize << endl;
                    remoteRank = (rank + 1 + (phase * 2)) %4;
                    for (unsigned int j = 0; j < innerRuntimeIterations; j++){
                        while (queue_request.size() >= pipelinedepth){
                            MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
                            queue_request.pop();
                        }
                        size_t index = (packagecount*j)%(buffersize/sizeof(int));
                        MPI_Issend((buffer + index), packagecount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &comm_obj);
                        //MPI_Wait(&comm_obj, MPI_STATUS_IGNORE);
                        queue_request.push(comm_obj);
                    }
                    while(!queue_request.empty()){
                        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
                        queue_request.pop();
                    }
                }
                else {
                    remoteRank = (rank + (size - 1) + (phase * 2)) %4;
                    //cout << rank << " receiving from " << remoteRank << " " << packagesize << endl;

                    for (unsigned int j = 0; j < innerRuntimeIterations; j++){
                        while (queue_request.size() >= pipelinedepth){
                            MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
                            queue_request.pop();
                        }
                        size_t index = (packagecount*j)%(buffersize/sizeof(int));
                        MPI_Irecv((buffer + index), packagecount, MPI_INT, remoteRank, j, MPI_COMM_WORLD, &comm_obj);
                        queue_request.push(comm_obj);
                        //MPI_Wait(&comm_obj, MPI_STATUS_IGNORE);
                    }
                    while(!queue_request.empty()){
                        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
                        queue_request.pop();
                    }
                }
                MPI_Barrier(MPI_COMM_WORLD);
                endtime = MPI_Wtime();
                if (rank%2==0){
                    cout << rank << " to " << remoteRank << " " << packagesize << " " << endtime-starttime << " " <<  (packagesize * innerRuntimeIterations)/((endtime-starttime)*1000000)<< endl;
                }
            }//packagesize
            cout << " " << endl;
        }//phase
        cout << " " << endl;
    }//iterations


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}

