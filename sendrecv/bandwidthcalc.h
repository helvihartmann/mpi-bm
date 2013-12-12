#ifndef BANDWIDTH
#define BANDWIDTH
#include <cstddef>

/* ~pfad
 27.11.13 class that performs all calculations on meassured time to gain bandwidth knowledge*/


class Bandwidthcalc{
private:
    double mean;
    double varianz;
    long double rate;
    /*double* time;
    int iterations;*/
    
    
public:
    
    
    double getmean(double*, int);
    /*Bandwidthcalc(double*,int);
    double getmean();*/
    
    long double getrate(double,size_t, size_t, size_t);

    double getvar(double*, int, double);
    //double getvar(double);

};
#endif