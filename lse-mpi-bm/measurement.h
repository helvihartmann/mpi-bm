# ifndef MEASUREMENT_H
#define MEASUREMENT_H
#include <iostream>
#include <mpi.h>
//#include "buffer.h"

enum method_t {basic, hist};
class Buffer;
class Measurement{
private:
    double starttime, endtime;
    
    
protected:
    MPI_Comm communicators_comm;
    MPI_Request comm_obj;
    size_t packagecount;
    
public:
    Measurement(MPI_Comm communicators_comm_);
        
    void warmup(Buffer *buffer, size_t numberofwarmups, size_t endpackagesize, int rank);
    
    void measure(Buffer *buffer, size_t packagecount_, size_t innerRuntimeIterations, enum method_t method);
    
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