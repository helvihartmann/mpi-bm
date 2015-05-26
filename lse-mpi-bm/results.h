#ifndef RESULTS_H
#define RESULTS_H
#include <iostream>
#include <vector>
#include <cmath>
/**/

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
    //std::vector<size_t>avg_waittestcounter;
public:
    
    Results(int rank, int statisticaliteration, int numberofpackages);
    
    void setvectors(int m, size_t idx_numberofpackages, size_t innerRuntimeIterations, size_t packagesize_tmp, int dataamountfactor,double time);
    
    void printstatisticaliteration();
    
    void calculate();
    
};

#endif /*RESULTS_H*/