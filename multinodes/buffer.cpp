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
    testwaitcounter.assign(numberofremoteranks,0);
    
    for(size_t j = 0; j < innerRuntimeIterations; j++){
        // wait for objects---------------------------
        while (queue_request.size() >= pipelinedepth*numberofremoteranks){
            MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
            testwaitcounter.front()+= timestamp.cycles();
            queue_request.pop();
        }
        // fill queue---------------------------------
        for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
            remoterank = remoterank_vec.at(index_remoterank);
            index = (packagecount*((j*numberofremoteranks)+index_remoterank))%(buffersize/sizeof(int));
            (*mpicall)((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
            queue_request.push(comm_obj);
        }
    }
    emptyqueue(queue_request);
}

void Buffer::comm_severalqueue(int (*mpicall)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*)){
    // iniate parameters--------------------------
    // vector holding all queues
    std::vector<queue<MPI_Request>> vec(numberofremoteranks);
    // flags
    int flag = 0;
    std::vector<size_t>count (numberofremoteranks,0);
    size_t countsum = 0;
    // fill queue-----------------------------------
    for(size_t j = 0; j < pipelinedepth; j++){
        for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
            remoterank = remoterank_vec.at(index_remoterank);
            index = packagecount*((j*numberofremoteranks)+index_remoterank)%(buffersize/sizeof(int));
            (*mpicall)((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
            vec[index_remoterank].push(comm_obj);
        }
    }
    // queue management-----------------------------
    while (countsum != (numberofremoteranks * innerRuntimeIterations)) {
        for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
            while (count.at(index_remoterank) != innerRuntimeIterations){ //loop over one receiver, while request objects say that sending is finished keeps sending new messages
                MPI_Test(&vec[index_remoterank].front(), &flag, MPI_STATUS_IGNORE);
                testwaitcounter.at(index_remoterank)++; //counts how often Wait was called for every receiver
                if (flag == 1){
                    vec[index_remoterank].pop();
                    index = packagecount*((count.at(index_remoterank)*numberofremoteranks)+index_remoterank)%(buffersize/sizeof(int));
                    remoterank = remoterank_vec.at(index_remoterank);
                   (*mpicall)((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
                    
                    count.at(index_remoterank)++;
                    countsum++;
                    vec[index_remoterank].push(comm_obj);
                }
                else {
                    break;//breaks out of while loop
                }
            }
        }
    }
    
    for(unsigned int index_remoterank = 0; index_remoterank < numberofremoteranks; index_remoterank++){
        emptyqueue(vec[index_remoterank]);
    }
}

//similar to above but with additional messurements for histogramms -----------------------------------------
void Buffer::comm_hist(int (*mpicall)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*)){
    std::queue<MPI_Request> queue_request;
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
            (*mpicall)((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
            commstop.at(j) = timestamp.stop();
            queue_request.push(comm_obj);
        }
    }
    emptyqueue(queue_request);
    
}

void Buffer::emptyqueue(std::queue<MPI_Request> queue_request){
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