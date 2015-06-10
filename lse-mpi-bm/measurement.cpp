#include "measurement.h"

Measurement::Measurement(size_t buffersize_, MPI_Comm communicators_comm_) :
    buffersize(buffersize_),
    communicators_comm(communicators_comm_)
{
    std::cout << "# allocating buffer..."  << std::endl;
    
    buffer = new int[(buffersize / sizeof(int))];
    
    std::cout << "# " << buffersize << " B allocated, initializing...\n" << std::endl;
    
    for (size_t i=0; i < (buffersize / sizeof(int)); i++){
        buffer[i]=0;
    }
    
    std::cout << "# buffer initialized.\n" << std::endl;
}


void Measurement::measure(std::vector<size_t> innerRuntimeIterations, std::vector<size_t>packagesizes, std::vector<int>remoterank_vec, int rank, unsigned int pipelinedepth, int flag, Results *results, unsigned int m){
    
    for (unsigned int z = 0; z < packagesizes.size(); z++){
        std::cout << z << " " << packagesizes.size() << " " << packagesizes.at(z) << " " << innerRuntimeIterations.size() << " " << innerRuntimeIterations.at(z) << std::endl;
        packagecount = packagesizes.at(z)/sizeof(int);
        MPI_Barrier(communicators_comm);
        starttime = MPI_Wtime();
        comm(remoterank_vec.size(), remoterank_vec, innerRuntimeIterations.at(z), pipelinedepth);
        
        MPI_Barrier(communicators_comm);
        endtime = MPI_Wtime();
        
        if (flag == 1){
            results->setvectors(m, z, innerRuntimeIterations.at(z), packagesizes.at(z), remoterank_vec.size(), endtime-starttime);

        }
        else if (flag == 0){
            //print bandwidth for warmup
            if (rank == 0){
                std::cout << innerRuntimeIterations.at(z)*packagecount*sizeof(int) << " " << innerRuntimeIterations.at(z) << " " << packagecount*sizeof(int) << " " << endtime - starttime << " - " << (innerRuntimeIterations.at(z)*packagecount*sizeof(int))/((endtime - starttime)*1000000) << " - " << rank << std::endl;
            }
        }
    }
    std::cout << " " << std::endl;
}

void Measurement::comm(unsigned int numberofremoteranks,  std::vector<int>remoterank_vec, size_t innerRuntimeIterations, unsigned int pipelinedepth){
    std::queue<MPI_Request> queue_request;
    size_t index;
    for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
        unsigned int remoterank = remoterank_vec.at(index_remoterank);
        for(size_t j = 0; j < innerRuntimeIterations; j++){
            // wait for objects---- pipelinesize scales with number of number of remoteranks
            while (queue_request.size() >= pipelinedepth*numberofremoteranks){
                MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
                queue_request.pop();
            }
            // fill queue---------------------------------
            index = (packagecount*((index_remoterank*innerRuntimeIterations)+j))%(buffersize/sizeof(int));

            MPI_Request comm_obj = mpisendrecvfunction(buffer, index, remoterank);
            queue_request.push(comm_obj);
        }
        MPI_Barrier(communicators_comm);
    }
    emptyqueue(queue_request);
}


void Measurement::emptyqueue(std::queue<MPI_Request> queue_request){
    while(!queue_request.empty()){
        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
        queue_request.pop();
    }
}