#include "measurement.h"
#include "buffer.h"

Measurement::Measurement(MPI_Comm communicators_comm_) :
    communicators_comm(communicators_comm_)
{
    
}


void Measurement::warmup(Buffer *buffer, size_t numberofwarmups, size_t endpackagesize, int rank){
    if (rank == 0){
        std::cout << "#warmup " << std::endl;
    }
    
    //iterate over packagesize from 1int (4Byte) to 16ki ints (64kiB)
    for (packagecount = 1; packagecount < endpackagesize; packagecount = packagecount*2){
        buffer->setloopvariables(packagecount, numberofwarmups);
        
        
        // data transfer and time measurement
        double starttimewarmup = MPI_Wtime();
        buffer->comm(this);
        double endtimewarmup = MPI_Wtime();
        
        //print bandwidth for warmup
        if (rank == 0){
            std::cout << numberofwarmups*packagecount*sizeof(int) << " " << numberofwarmups << " " << packagecount*sizeof(int) << " " << endtimewarmup - starttimewarmup << " - " << (numberofwarmups*packagecount*sizeof(int))/((endtimewarmup - starttimewarmup)*1000000) << " - " << rank << std::endl;
        }
        
    }
    std::cout << " " << std::endl;
}

void Measurement::measure(Buffer *buffer, size_t packagecount_, size_t innerRuntimeIterations, enum method_t method){
    packagecount = packagecount_;
    buffer->setloopvariables(packagecount,innerRuntimeIterations);
    MPI_Barrier(communicators_comm);
    starttime = MPI_Wtime();
    switch (method) {
        case basic:
             buffer->comm(this);
            break;
        case hist:
            buffer->comm_hist(this);
            break;
        default:
            buffer->comm(this);
            break;
    }
   
    MPI_Barrier(communicators_comm);
    endtime = MPI_Wtime();

}