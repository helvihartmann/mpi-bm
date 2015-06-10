# ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <iostream>
#include <mpi.h>
#include <queue>
#include <vector>

//#include "buffer.h"
using namespace std;
class DataManager{
private:
    size_t buffersize;
    int * buffer;
    unsigned int pipelinedepth = 8;
    vector<double>time;
protected:
    MPI_Comm communicators_comm;
    MPI_Request comm_obj;
    size_t packagecount;
    
public:
    DataManager(size_t buffersize);
    
    vector<double> run(vector<size_t>packagesize, vector<size_t> innerRuntimeIterations_, vector<int> remoteranks, size_t numberofwarmups, int commflag, int nmbr_commprocess);
    
    void sendrecvdata(unsigned int remoterank, size_t innerRuntimeIterations);
    
    virtual MPI_Request mpisendrecvfunction(int *buffer, size_t index, unsigned int remoterank)=0;
};



//------------------------------------
class Measurementsend : public DataManager {
    
public:
    using DataManager::DataManager;
    MPI_Request mpisendrecvfunction(int *buffer, size_t index, unsigned int remoterank) override {
        MPI_Issend((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
        return comm_obj;
    }
};

class Measurementrecv : public DataManager {
    
public:
    using DataManager::DataManager;
    
    MPI_Request mpisendrecvfunction(int *buffer, size_t index, unsigned int remoterank) override {
        MPI_Irecv((buffer + index), packagecount, MPI_INT, remoterank, 1, MPI_COMM_WORLD, &comm_obj);
        return comm_obj;
    }
};


class Measurementobserver : public DataManager {
    
public:
    using DataManager::DataManager;
    MPI_Request mpisendrecvfunction(int *buffer, size_t index, unsigned int remoterank) override {
        cout << "ERROR Observer do not call this function where data is send. Something went wrong at buffer pointing to " << buffer + index << " sending to " << remoterank << endl;
        return 0;
    }
};

#endif