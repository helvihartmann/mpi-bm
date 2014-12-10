#include <iostream>
#include "results.h"

class Output{
private:
    int rank, size;
    MPI_Comm communicators_comm;
    
public:
    
    Output(int rank_, int size_,  MPI_Comm communicators_comm_);
    void outputiteration(Results *results, unsigned int m);
    void outputfinal(Results *results, int commflag);
    
};

Output::Output(int rank_, int size_, MPI_Comm communicators_comm_) :
    rank(rank_),
    size(size_),
    communicators_comm(communicators_comm_)
{

    
}

void Output::outputiteration(Results *results, unsigned int m){
    MPI_Barrier(communicators_comm);
    for (int i=0; i<size; i++) {
        if (rank == i){
            if (rank == 0){
                cout << m << ". iteration-------------------" << endl;
                
            }
            results->printstatisticaliteration();
            sleep(2);
        }
        MPI_Barrier(communicators_comm);
    }
}

void Output::outputfinal(Results *results, int commflag){
    for (int i=0; i<size; i++) {
        if (rank == i){
            if(rank == 0){
                cout << "# processes " << size << endl;
            }
            
            if (commflag == 0){
                cout << "#----------------------- SENDER ---------------------------" << endl;
                cout << "# totaldatasent repeats  packagesize time [us] std sendbandwidth [MB/s] std \n" << endl;
            }
            
            else {
                cout << "#--- RECEIVER ----------------------------" << endl;
            }
            
            results->calculate();
            cout << "\n\n" << endl;
            sleep(5);
        }
        MPI_Barrier(communicators_comm);
        
    }
}