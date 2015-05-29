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


//similar to above but with additional messurements for histogramms -----------------------------------------
void CommunicationManager::comm_hist(Measurement *measurement){
    std::queue<MPI_Request> queue_request;
    
    commstart.resize(innerRuntimeIterations);
    commstop.resize(innerRuntimeIterations);
    
    waitstart.resize(innerRuntimeIterations);
    waitstop.resize(innerRuntimeIterations);
    
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        // wait for objects---------------------------
        while (queue_request.size() >= pipelinedepth*numberofremoteranks){
            waitstart.at(j) = timestamp.start();
            MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
            waitstop.at(j) = timestamp.stop();
            queue_request.pop();
        }
        // fill queue---------------------------------
        for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
            remoterank = remoterank_vec.at(index_remoterank);
            index = (packagecount*((j*numberofremoteranks)+index_remoterank))%(buffersize/sizeof(int));
            commstart.at(j) = timestamp.start();

            MPI_Request comm_obj = measurement->mpisendrecvfunction(buffer, index, remoterank);
            commstop.at(j) = timestamp.stop();
            queue_request.push(comm_obj);
        }
    }
    emptyqueue(queue_request);
}

void CommunicationManager::emptyqueue(std::queue<MPI_Request> queue_request){
    while(!queue_request.empty()){
        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
        queue_request.pop();
    }
}


void CommunicationManager::printsingletime(){
    size_t i = 0;
    ostringstream oss;
    oss << (packagecount*sizeof(int)) << "_x" << numberofremoteranks << "_p" << pipelinedepth << "_n" << size << "_" << rank << ".hist";
    ofstream myfile;
    myfile.open(oss.str());

    myfile << "# P(s)IS P(e)Is T[ms] P(s)Ws P(e)Ws TWs P(s)RS P(e)Rs T[ms]" << "\n";
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        myfile << commstart.at(j) << " " << commstop.at(j) << " " << ((commstop.at(j)-commstart.at(j))/2) << " " << waitstart.at(j) << " " << waitstop.at(j) << " " << ((waitstop.at(j)-waitstart.at(j))/2) <<  " " << i << "\n";
        i++;
    }
    myfile.close();
}