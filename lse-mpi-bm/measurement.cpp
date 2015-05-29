#include "measurement.h"
#include "communicationManager.h"

Measurement::Measurement(CommunicationManager *datahandle_, MPI_Comm communicators_comm_) :
    datahandle(datahandle_),
    communicators_comm(communicators_comm_)
{
    
}

void Measurement::warmup(size_t numberofwarmups, size_t endpackagesize, int rank){
    if (rank == 0){
        std::cout << "#warmup " << std::endl;
    }
    
    //iterate over packagesize from 1int (4Byte) to 16ki ints (64kiB)
    for (packagecount = 1; packagecount < endpackagesize; packagecount = packagecount*2){
        datahandle->setloopvariables(packagecount, numberofwarmups);
        
        
        // data transfer and time measurement
        double starttimewarmup = MPI_Wtime();
        datahandle->comm(this);
        double endtimewarmup = MPI_Wtime();
        
        //print bandwidth for warmup
        if (rank == 0){
            std::cout << numberofwarmups*packagecount*sizeof(int) << " " << numberofwarmups << " " << packagecount*sizeof(int) << " " << endtimewarmup - starttimewarmup << " - " << (numberofwarmups*packagecount*sizeof(int))/((endtimewarmup - starttimewarmup)*1000000) << " - " << rank << std::endl;
        }
        
    }
    std::cout << " " << std::endl;
}

void Measurement::measure(size_t packagecount_, size_t innerRuntimeIterations, enum method_t method){
    packagecount = packagecount_;
    datahandle->setloopvariables(packagecount,innerRuntimeIterations);
    MPI_Barrier(communicators_comm);
    starttime = MPI_Wtime();
    switch (method) {
        case basic:
             datahandle->comm(this);
            break;
        case hist:
            datahandle->comm_hist(this);
            break;
        default:
            datahandle->comm(this);
            break;
    }
   
    MPI_Barrier(communicators_comm);
    endtime = MPI_Wtime();
}