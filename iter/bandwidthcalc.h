#ifndef BANDWIDTH
#define BANDWIDTH
#include <cstddef>
#include "totaldatasendcalc.h"
#include "mpi.h"

/* ~pfad
 27.11.13 class that performs all calculations on meassured time to gain bandwidth knowledge*/


class Bandwidthcalc{
private:
    double mean;
    double varianz;
    long double rate;
    double* time;
    int iterations;
    
public:
    Bandwidthcalc(double*, int);
    
    double getmean();
    
    long double getrate(size_t);
    
    double getvar();

};
#endif