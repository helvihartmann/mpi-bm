#include "totaldatasendcalc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

/* pfad
27.11.2013
*/

void Totaldatasendcalc::setPackagesizeTmp(size_t p){
    packagesize_temp = p;//correct at this point
}

void Totaldatasendcalc::readOptions(int &argc, char **argv){
    int opt;

    //int opterr = 0;

    while ((opt = getopt (argc, argv, "m:a:i:e:o:")) != -1)
        switch (opt)
    {
        case 'm':
            sendmode = atoi(optarg);
            if (sendmode >= 1 && sendmode <= 3) {
                //zwischen 1 und 3
            }
            else {
                printf("ERROR \n-m: your options are 1 send; 2 Ssend, 3 Bsend\n");
                exit(1);
            }
            break;
        case 'a':
            startPackageSize = atoi(optarg);
            
            if (startPackageSize >= 1 && startPackageSize<= 10000000000) {//10GiB max
            }
            else {
                printf("ERROR \n-a: please enter vaild number for package size, which is not supposed to exceed 10GiB\n");
                exit(1);
            }
            break;
        case 'i':
            startiteration = atoi(optarg);
            if (startiteration >=1 && startiteration<= 100000000) {
            }
            else {
                startiteration = 20000000*128;
                printf("#INFO \n-i: too many startiteration; limited to 128*1mio \n");
            }
            break;
        case 'e':
            cutoff = atoi(optarg);
            if (cutoff >= 1 && cutoff <= startiteration) {
            }
            else {
                cutoff = startiteration;
                printf("#INFO \n-e: max package size was set to %ld B \n",startiteration);
            }
            break;
        case 'o':
            statisticaliterations = atoi(optarg);
            if (statisticaliterations >=1 && statisticaliterations <= 1000) {
            }
            else {
                statisticaliterations=1000;
                printf("#INFO \n-o: statistical iterations were limited to 1000 \n");
            }
            break;
        case '?':
            fprintf (stderr,
                     "ERROR: Unknown option character `\\x%x'.\n",
                     optopt);
        default:
            abort ();
    }
}

size_t Totaldatasendcalc::getpackagesize(){
        return startPackageSize;
}
        
size_t Totaldatasendcalc::getcutoff(){
        return cutoff;
}

int Totaldatasendcalc::getsendmode(){
    return sendmode;
}

int Totaldatasendcalc::getstatisticaliterations(){
    return statisticaliterations;
}


size_t Totaldatasendcalc::getinnerRuntimeIterations(){
    if(startiteration==1){
        iterations=1;
    }
    else {
        iterations = startiteration/packagesize_temp;
    }
    return iterations;
}
    
size_t Totaldatasendcalc::getTotalDataSent(){
    size_t totalDataSent = iterations*packagesize_temp*sizeof(int);
    return totalDataSent;
}
