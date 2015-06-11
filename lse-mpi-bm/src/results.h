#ifndef RESULTS_H
#define RESULTS_H
#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <unistd.h>
/*2015 Copyright Helvi Hartmann <hhartmann@fias.uni-frankfurt.de
 collects time measured to transfer data and calculates data rate from it*/
using namespace std;
class Results{
private:
    int rank;
    int statisticaliteration;
    int numberofpackages;
    
    std::vector<size_t>package_vector;
    std::vector<size_t>innerRuntimeIterations_vector;
    std::vector<size_t>totaldatasent_vector;
    
    int idx_outeriter;
    int numberofRemoteranks;
    std::vector<double> time;
    std::vector<double> summe;

    MPI_Comm communicators_comm;
    int size;
public:
    
    Results(int rank, int statisticaliteration, int numberofpackages, MPI_Comm communicators_comm_, int size_);
    
    void setvectors(int m, size_t idx_numberofpackages, size_t innerRuntimeIterations, size_t packagesize_tmp, int dataamountfactor,double time);
    
    void outputiteration(unsigned int m);
    void outputfinal(int commflag);
    
    void printstatisticaliteration();
    
    void calculate();
    
};

#endif /*RESULTS_H*/