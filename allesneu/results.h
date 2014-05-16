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
    
    std::vector<double> time;
    std::vector<double> summe;
    
public:
    
    Results(int rank, int statisticaliteration, int numberofpackages);
    
    void setvectors(int m, size_t idx_numberofpackages, size_t innerRuntimeIterations, size_t packagesize_tmp, double time);
    
    void calculate();
    
};

#endif /*RESULTS_H*/