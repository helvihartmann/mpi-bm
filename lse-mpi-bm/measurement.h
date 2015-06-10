# ifndef MEASUREMENT_H
#define MEASUREMENT_H
#include <iostream>
#include <mpi.h>
#include <queue>
#include "results.h"

//#include "buffer.h"

class Measurement{
private:
    size_t buffersize;
    double starttime, endtime;
    int *buffer;

protected:
    MPI_Comm communicators_comm;
    MPI_Request comm_obj;
    size_t packagecount;
    
public:
    Measurement(size_t buffersize, MPI_Comm communicators_comm_);

    void measure(std::vector<size_t> innerRuntimeIterations, std::vector<size_t>packagesizes, std::vector<int>remoterank_vec, int rank, unsigned int pipelinedepth, int flag, Results *results, unsigned int m);
    
    void comm(unsigned int numberofremoteranks, std::vector<int>remoterank_vec, size_t innerRuntimeIterations, unsigned int pipelinedepth);
    
    void emptyqueue(std::queue<MPI_Request> queue_request);
    
    
    virtual MPI_Request mpisendrecvfunction(int *buffer, size_t index, unsigned int remoterank)=0;
    double getstarttime() { return starttime; }
    
    double getendtime() { return endtime; }
};

class Measurementsend : public Measurement {
    
public:
    using Measurement::Measurement;
    
    MPI_Request mpisendrecvfunction(int *buffer, size_t index, unsigned int remoterank) override {
        MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, communicators_comm, &comm_obj);
        return comm_obj;
    }
};

class Measurementrecv : public Measurement {
    
public:
    using Measurement::Measurement;
    
    MPI_Request mpisendrecvfunction(int *buffer, size_t index, unsigned int remoterank) override {
        MPI_Irecv((buffer + index), packagecount, MPI_INT, remoterank, 1, communicators_comm, &comm_obj);
        return comm_obj;
    }
};
#endif