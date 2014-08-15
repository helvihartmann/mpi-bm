#include <iostream>
#include "results.h"

class Output{
private:
    int rank, size;
    
public:
    
    Output(int rank_, int size_);
    void outputiteration(Results *results, unsigned int m);
    void outputfinal(Results *results, int commflag);
};

Output::Output(int rank_, int size_) :
    rank(rank_),
    size(size_)
{

    
}

void Output::outputiteration(Results *results, unsigned int m){
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i=0; i<size; i++) {
        if (rank == i){
            if (rank == 0){
                cout << m << ". iteration-------------------" << endl;
                
            }
            results->printstatisticaliteration();
            sleep(2);
        }
        MPI_Barrier(MPI_COMM_WORLD);
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
                results->calculate();
                cout << "\n\n" << endl;
                
            }
            
            else {
                cout << "#--- RECEIVER ----------------------------" << endl;
                results->calculate();
                cout << "\n\n" << endl;
            }
            sleep(5);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        
    }
}