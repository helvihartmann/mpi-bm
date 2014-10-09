#include <iostream>
#include "buffer.h"

enum method_t {basic, hist, sev_queue};

class Measurement{
private:
    int (*mpicall)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*);
    
    double starttime, endtime;
    Buffer *buffer;
    
public:
    Measurement(Buffer *buffer_);
    
    void setfunctionpointer(int (*mpicall_)(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request*));
    
    void warmup(size_t numberofwarmups, size_t endpackagesize, int rank);
    
    void measure(size_t packagecount, size_t innerRuntimeIterations, enum method_t method);
    
    double getstarttime() { return starttime; }
    
    double getendtime() { return endtime; }
};