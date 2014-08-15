#include <iostream>
#include "buffer.h"

class Measurement{
private:
    int (*mpicall)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*);
    
    double starttime, endtime;
    Buffer *buffer;
    
public:
    Measurement(Buffer *buffer_);
    
    void setfunctionpointer(int (*mpicall_)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
    
    void warmup(size_t numberofwarmups, int rank);
    
    void measure(size_t packagecount, size_t innerRuntimeIterations);
    
    void measure_hist(size_t packagecount, size_t innerRuntimeIterations);
    
    double getstarttime() { return starttime; }
    
    double getendtime() { return endtime; }
};

Measurement::Measurement(Buffer *buffer_) :
    buffer(buffer_)
{
    
}

void Measurement::setfunctionpointer(int (*mpicall_)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*)){
    mpicall = mpicall_;
}

void Measurement::warmup(size_t numberofwarmups, int rank){
    if (rank == 0){
        std::cout << "#warmup " << std::endl;
    }
    
    for (size_t packagecount = 1; packagecount < 1<<24; packagecount = packagecount*2){
        buffer->setloopvariables(packagecount, numberofwarmups);
        
        double starttime = MPI_Wtime();
        buffer->comm(mpicall);
        double endtime = MPI_Wtime();
        
        if (rank == 0){
            std::cout << numberofwarmups*packagecount*sizeof(int) << " " << numberofwarmups << " " << packagecount*sizeof(int) << " " << endtime - starttime << " - " << (numberofwarmups*packagecount*sizeof(int))/((endtime - starttime)*1000000) << " - " << rank << std::endl;
        }
        
    }
    std::cout << " " << std::endl;
}

void Measurement::measure(size_t packagecount, size_t innerRuntimeIterations){
    buffer->setloopvariables(packagecount,innerRuntimeIterations);
    MPI_Barrier(MPI_COMM_WORLD);
    starttime = MPI_Wtime();
    buffer->comm(mpicall);
    MPI_Barrier(MPI_COMM_WORLD);
    endtime = MPI_Wtime();

}

void Measurement::measure_hist(size_t packagecount, size_t innerRuntimeIterations){
    buffer->setloopvariables(packagecount,innerRuntimeIterations);
    MPI_Barrier(MPI_COMM_WORLD);
    starttime = MPI_Wtime();
    buffer->comm_hist(mpicall);
    MPI_Barrier(MPI_COMM_WORLD);
    endtime = MPI_Wtime();
    
}