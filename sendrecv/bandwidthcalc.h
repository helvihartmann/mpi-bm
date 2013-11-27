//#include <stdio.h>
//#include <cmath>
#include <cstddef>

/* ~pfad
 27.11.13 class that performs all calculations on meassured time to gain bandwidth knowledge*/


class Bandwidthcalc{
private:
    double mean;
    double varianz;
    long double rate;
    
public:
    double getmean(double*, int);
    
    long double getrate(double,size_t, size_t, size_t);

    double getvar(double*, int, double);
};