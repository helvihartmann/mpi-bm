//#include <stdio.h>
#include <cmath>
#include "bandwidthcalc.h"

/* ~pfad
 27.11.13 class that performs all calculations on meassured time to gain bandwidth knowledge*/

/*Bandwidthcalc::Bandwidthcalc(double* time, int iterations){
    iterations = iterations;
}*/

Bandwidthcalc::Bandwidthcalc(double time[], int iterations){
    time = time;
    iterations = iterations;
}

double Bandwidthcalc::getmean(){
    double sum = 0;
    for(int m=0; m<iterations; m++){
        double mean_tmp = time[m];
        sum = sum + mean_tmp;
    }
    double mean = sum/iterations;
    return mean;
}

long double Bandwidthcalc::getrate(size_t iterations2, size_t packagesize_temp, size_t totaldatasent){
    rate = totaldatasent/mean;
    return rate;
}

double Bandwidthcalc::getvar(){
    double varianz_tmp = 0;
    int m;
    for( m=0; m<iterations; m++){
        double mean_tmp = time[m];
        double diff = pow((mean - mean_tmp),2);
        varianz_tmp = varianz_tmp + diff;
    }
    varianz = sqrt(varianz_tmp)/m;
    return varianz;
}


/*double Bandwidthcalc::getmean(double time[], int iterations){
        double sum = 0;
        for(int m=0; m<iterations; m++){
            double mean_tmp = time[m];
            sum = sum + mean_tmp;
        }
        double mean = sum/iterations;
        return mean;
}
    
long double Bandwidthcalc::getrate(double mean,size_t iterations2, size_t packagesize_temp, size_t totaldatasent){
        rate = totaldatasent/mean;
        return rate;
}

double Bandwidthcalc::getvar(double time[], int iterations, double mean){
        double varianz_tmp = 0;
        int m;
        for( m=0; m<iterations; m++){
            double mean_tmp = time[m];
            double diff = pow((mean - mean_tmp),2);
            varianz_tmp = varianz_tmp + diff;
        }
        varianz = sqrt(varianz_tmp)/m;
        return varianz;
}*/