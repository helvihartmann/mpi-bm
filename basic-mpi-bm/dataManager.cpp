#include "dataManager.h"


DataManager::DataManager(size_t buffersize_) :
    buffersize(buffersize_)
{
    buffer = new int[(buffersize / sizeof(int))];
    
    std::cout << "# " << buffersize << " B allocated, initializing...\n" << std::endl;
    
    for (size_t i=0; i < (buffersize / sizeof(int)); i++){
        buffer[i]=0;
    }
    
    std::cout << "# buffer initialized.\n" << std::endl;

}

vector<double> DataManager::run(vector<size_t>packagesize, vector<size_t> innerRuntimeIterations_, int rank, size_t nmbr_warmups, int commflag, unsigned int nmbr_commprocess){
    unsigned int remoteRank = 0;
    double starttime = 0;
    double endtime = 0;
    time.clear();
    size_t innerRuntimeIteration;
    for(unsigned int phase = 0; phase < (nmbr_commprocess/2); phase++){
        for (int warmup = 0; warmup < 2; warmup++){
            for (unsigned int i = 0; i < packagesize.size(); i++){
                if (warmup == 0){
                    innerRuntimeIteration = nmbr_warmups;
                }
                else{
                    innerRuntimeIteration = innerRuntimeIterations_.at(i);
                }
                packagecount = packagesize.at(i)/sizeof(int);
                MPI_Barrier(MPI_COMM_WORLD);
                if (commflag < 2){
                    starttime = MPI_Wtime();
                    if (rank%2 == 0){
                        remoteRank = (rank + 1 + (phase * 2)) % nmbr_commprocess;
                        //cout << phase << " " << rank << " sending to " << remoteRank << " " << packagecount << endl;
                        sendrecvdata(remoteRank, innerRuntimeIteration);
                    }
                    else {
                        remoteRank = (rank + (nmbr_commprocess - 1) - (phase * 2)) % nmbr_commprocess;
                        sendrecvdata(remoteRank, innerRuntimeIteration);
                    }
                }
                MPI_Barrier(MPI_COMM_WORLD);
                endtime = MPI_Wtime();
                if (warmup !=0){
                    time.push_back(endtime - starttime);
                }
            }
        }

        }
    return time;
}

void DataManager::sendrecvdata(unsigned int remoterank, size_t innerRuntimeIterations){
    std::queue<MPI_Request> queue_request;
    for (unsigned int j = 0; j < innerRuntimeIterations; j++){
        while (queue_request.size() >= pipelinedepth){
            MPI_Wait (&queue_request.front(), MPI_STATUS_IGNORE);
            queue_request.pop();
        }
        size_t index = (packagecount*j)%(buffersize/sizeof(int));
        comm_obj = mpisendrecvfunction(buffer, index, remoterank);
        queue_request.push(comm_obj);
    }
    
    while(!queue_request.empty()){
        MPI_Wait(&queue_request.front(), MPI_STATUS_IGNORE);
        queue_request.pop();
    }

}
