#include "buffer.h"

Buffer::Buffer(int size_, int rank_, unsigned int pipelinedepth_, size_t buffersize_, std::vector<int>remoterank_vec_, unsigned int numberofremoteranks_) :
    size(size_),
    rank(rank_),
    pipelinedepth(pipelinedepth_),
    buffersize(buffersize_),
    remoterank_vec(remoterank_vec_),
    numberofremoteranks(numberofremoteranks_)
{
    std::cout << "# allocating buffer..." << rank << std::endl;
    
    buffer = new int[(buffersize / sizeof(int))];
    
    std::cout << "# " << buffersize << " B allocated, initializing...\n" << std::endl;
    
    for (size_t i=0; i < (buffersize / sizeof(int)); i++){
        buffer[i]=0;
    }
    
    std::cout << "# buffer initialized.\n" << std::endl;
}

void Buffer::setloopvariables(size_t packagecount_, size_t innerRuntimeIterations_){
    innerRuntimeIterations = innerRuntimeIterations_;
    packagecount = packagecount_;
}

void Buffer::comm(int (*mpicall)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*)){
    std::queue<MPI_Request> queue_request;
    MPI_Request comm_obj;
    int remoterank;
    size_t index = 0;
    testwaitcounter.assign(numberofremoteranks,0);
    
    int remoterankflag, index_remoterank;
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        // wait for objects---------------------------
        while (queue_request.size() >= pipelinedepth*numberofremoteranks){
            MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
            testwaitcounter.front()+= timestamp.cycles();
            queue_request.pop();
        }
        // fill queue---------------------------------
        /*for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
            remoterank = remoterank_vec.at(index_remoterank);
            index = (packagecount*((j*numberofremoteranks)+index_remoterank))%(buffersize/sizeof(int));
            (*mpicall)((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
            queue_request.push(comm_obj);
        }*/
        remoterankflag = 0;
        while(remoterankflag < numberofremoteranks){
            index_remoterank = (rank + remoterankflag)%numberofremoteranks;
            remoterank = remoterank_vec.at(index_remoterank);
            index = (packagecount*((j*numberofremoteranks)+index_remoterank))%(buffersize/sizeof(int));
            (*mpicall)((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
            queue_request.push(comm_obj);
            remoterankflag++;
        }
    }
    // empty queue---------------------------------
    while(!queue_request.empty()){
        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
        queue_request.pop();
    }
}

//similar to above but with additional messurements for histogramms -----------------------------------------
void Buffer::comm_hist(int (*mpicall)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*)){
    std::queue<MPI_Request> queue_request;
    MPI_Request send_obj;
    int remoterank;
    size_t index = 0;
    testwaitcounter.assign(numberofremoteranks,0);
    
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
            testwaitcounter.front()+= timestamp.cycles();
            queue_request.pop();
        }
        // fill queue---------------------------------
        for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
            remoterank = remoterank_vec.at(index_remoterank);
            index = (packagecount*((j*numberofremoteranks)+index_remoterank))%(buffersize/sizeof(int));
            commstart.at(j) = timestamp.start();
            (*mpicall)((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &send_obj);
            commstop.at(j) = timestamp.stop();
            queue_request.push(send_obj);
        }
    }
    // empty queue---------------------------------
    while(!queue_request.empty()){
        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
        queue_request.pop();
    }
}


void Buffer::printsingletime(){
    size_t i = 0;
    ostringstream oss;
    oss << (packagecount*sizeof(int)) << "_x" << numberofremoteranks << "_p" << pipelinedepth << "_n" << size << "_" << rank << ".hist";
    ofstream myfile;
    myfile.open(oss.str());

    myfile << "# P(s)IS P(e)Is TIS P(s)Ws P(e)Ws TWs P(s)RS P(e)Rs TIS" << "\n";
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        myfile << commstart.at(j) << " " << commstop.at(j) << " " << ((commstop.at(j)-commstart.at(j))/2000) << " " << waitstart.at(j) << " " << waitstop.at(j) << " " << ((waitstop.at(j)-waitstart.at(j))/2000) <<  " " << i << "\n";
        i++;
    }
    
    myfile.close();
}