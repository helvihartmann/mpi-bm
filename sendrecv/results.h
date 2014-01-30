#ifndef RESULTS_H
#define RESULTS_H

#include <vector>
#include <array>
#include <cmath>
#include <iostream>
/* ~/benchmark/sendrecv/calculatebandwidth.cpp
 23.01.14*/

class Results{
private:
    int outerStatisticalIterations;
    int numberofpackages;
    std::vector<double> summe;
    std::vector<double> time;
    
    std::vector<size_t>package_vector;
    std::vector<size_t>innerRuntimeIterations_vector;
    std::vector<size_t>totaldatasent_vector;
    //std::vector<double> loadavg_vector;
    
    
    //std::array<std::array<double,outerStatisticalIterations>,numberofpackages> time;
public:
    
    Results(int, int);
    
    void setvectors(size_t packagesize_tmp,
                             size_t innerRuntimeIterations,
                             int idx_numberofpackages);
    
    void settime(int idx_outeriter,
                 int idx_numberofpackages,
                 double time_);
    
    void calculate();
};

#endif /*BUFFEROPERATIONS_H*/