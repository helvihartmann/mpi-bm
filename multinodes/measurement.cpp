#include "measurement.h"

Measurement::Measurement(Buffer *buffer_) :
    buffer(buffer_)
{
    
}

void Measurement::setfunctionpointer(int (*mpicall_)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*)){
    mpicall = mpicall_;
}

void Measurement::warmup(size_t numberofwarmups, size_t endpackagesize, int rank){
    if (rank == 0){
        std::cout << "#warmup " << std::endl;
    }
    
    //iterate over packagesize from 1int (4Byte) to 16ki ints (64kiB)
    for (size_t packagecount = 1; packagecount < endpackagesize; packagecount = packagecount*2){
        buffer->setloopvariables(packagecount, numberofwarmups);
        
        // data transfer and time measurement
        double starttimewarmup = MPI_Wtime();
        buffer->comm(mpicall);
        double endtimewarmup = MPI_Wtime();
        
        //print bandwidth for warmup
        if (rank == 0){
            std::cout << numberofwarmups*packagecount*sizeof(int) << " " << numberofwarmups << " " << packagecount*sizeof(int) << " " << endtimewarmup - starttimewarmup << " - " << (numberofwarmups*packagecount*sizeof(int))/((endtimewarmup - starttimewarmup)*1000000) << " - " << rank << std::endl;
        }
        
    }
    std::cout << " " << std::endl;
}

void Measurement::measure(size_t packagecount, size_t innerRuntimeIterations, enum method_t method, MPI_Comm communicators_comm){
    buffer->setloopvariables(packagecount,innerRuntimeIterations);
    MPI_Barrier(communicators_comm);
    starttime = MPI_Wtime();
    switch (method) {
        case basic:
             buffer->comm(mpicall);
            break;
        case hist:
            buffer->comm_hist(mpicall);
            break;
        case sev_queue:
            buffer->comm_severalqueue(mpicall);
            break;
        default:
            buffer->comm(mpicall);
            break;
    }
   
    MPI_Barrier(communicators_comm);
    endtime = MPI_Wtime();

}