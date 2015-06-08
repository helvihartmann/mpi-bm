#include "communicationManager.h"

CommunicationManager::CommunicationManager(int size_, int rank_, unsigned int pipelinedepth_, size_t buffersize_, std::vector<int>remoterank_vec_, unsigned int numberofremoteranks_, MPI_Comm communicators_comm_) :
    size(size_),
    rank(rank_),
    pipelinedepth(pipelinedepth_),
    buffersize(buffersize_),
    remoterank_vec(remoterank_vec_),
    numberofremoteranks(numberofremoteranks_),
    communicators_comm(communicators_comm_)
{
    std::cout << "# allocating buffer..." << rank << std::endl;
    
    buffer = new int[(buffersize / sizeof(int))];
    
    std::cout << "# " << buffersize << " B allocated, initializing...\n" << std::endl;
    
    for (size_t i=0; i < (buffersize / sizeof(int)); i++){
        buffer[i]=0;
    }
    
    std::cout << "# buffer initialized.\n" << std::endl;
}

void CommunicationManager::setloopvariables(size_t packagecount_, size_t innerRuntimeIterations_){
    innerRuntimeIterations = innerRuntimeIterations_;
    packagecount = packagecount_;
}

void CommunicationManager::comm(Measurement *measurement){
    std::queue<MPI_Request> queue_request;

    for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
        remoterank = remoterank_vec.at(index_remoterank);
        for(size_t j = 0; j < innerRuntimeIterations; j++){
            // wait for objects---- pipelinesize scales with number of number of remoteranks
            while (queue_request.size() >= pipelinedepth*numberofremoteranks){
                MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
                queue_request.pop();
            }
            // fill queue---------------------------------
            index = (packagecount*((index_remoterank*innerRuntimeIterations)+j))%(buffersize/sizeof(int));
            MPI_Request comm_obj = measurement->mpisendrecvfunction(buffer, index, remoterank);
            queue_request.push(comm_obj);
        }
        MPI_Barrier(communicators_comm);
    }
    emptyqueue(queue_request);
}


void CommunicationManager::emptyqueue(std::queue<MPI_Request> queue_request){
    while(!queue_request.empty()){
        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
        queue_request.pop();
    }
}