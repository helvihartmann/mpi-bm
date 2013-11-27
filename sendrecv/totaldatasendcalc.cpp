#include "totaldatasendcalc.h"

/* pfad
27.11.2013
*/

void Totaldatasendcalc::setPackagesizeTmp(size_t p){
        packagesize_temp = p;
}
    
size_t Totaldatasendcalc::getpackagesize(int &argc, char **argv){
        size_t ncounts;
        if (argc > 0){
            ncounts = atoll(argv[1]);
        }
        else{
            ncounts = 128;
        }
        return ncounts;
}
        
size_t Totaldatasendcalc::getcutoff(int &argc, char **argv){
        size_t ncounts;
        if (argc > 1){
            cutoff = atoll(argv[2]);
        }
        else{
            cutoff = 8000000;
        }
        return cutoff;
}

size_t Totaldatasendcalc::getiterations2(){
        iterations2 = empiricalfactor/packagesize_temp;
        return iterations2;
}
    
size_t Totaldatasendcalc::getTotalDataSent(){
        size_t totalDataSent = iterations2*packagesize_temp*sizeof(int);
        return totalDataSent;
}