#ifndef CALCULATEBANDWIDTH_H
#define CALCULATEBANDWIDTH_H

#include "classmpi3.h"
#include <vector>
#include <array>
#include <cmath>
/* ~/benchmark/sendrecv/calculatebandwidth.cpp
 23.01.14*/

class Calculatebandwidth{
private:
    int outerStatisticalIterations;
    int numberofpackages;
    std::vector<double> summe;
    //std::vector<size_t>package_vector;
    //std::vector<size_t>innerRuntimeIterations_vector;
    //std::vector<size_t>totaldatasent_vector;
    std::vector<double> loadavg_vector;
    
    std::vector<double> time;
    //std::array<std::array<double,outerStatisticalIterations>,numberofpackages> time;
public:
    
    Calculatebandwidth(int, int);
    
    void settime(int idx_outeriter,
                 int idx_numberofpackages,
                 double time_);
    
    void calculate(std::vector<size_t> &totaldatasent_vector,
                   std::vector<size_t>&package_vector,
                   std::vector<size_t>&innerRuntimeIterations_vector,std::vector<double> &loadavg_vector);
};

#endif /*BUFFEROPERATIONS_H*/