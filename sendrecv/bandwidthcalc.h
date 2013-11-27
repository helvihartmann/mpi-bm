//#include <stdio.h>
//#include <cmath>

/* ~pfad
 20.11.13 class that performs all calculations on meassured time to gain bandwidth knowledge*/


class Bandwidthcalc{
private:
    double mean;
    double varianz;
    long double rate;
    
public:
    double getmean(double time[], int iterations){
        double sum = 0;
        for(int m=0; m<iterations; m++){
            double mean_tmp = time[m];
            sum = sum + mean_tmp;
        }
        double mean = sum/iterations;
        return mean;
    }
    
    long double getrate(double mean,size_t iterations2, size_t packagesize_temp, size_t totaldatasent){
        rate = totaldatasent/mean;
        return rate;
    }

    double getvar(double time[], int iterations, double mean){
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
};