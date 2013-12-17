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

    while ((opt = getopt (argc, argv, "m:a:e:")) != -1)
    switch (opt)
    {
        case 'm':
            sendmode = atoi(optarg);
            if (sendmode >= 1 && sendmode <= 3) {
                //zwischen 1 und 3
            }
            else {
                printf("FAILURE \n-m: your options are 1 send; 2 Ssend, 3 Bsend\n");
                exit(1);
            }
            break;
        case 'a':
            startPackageSize = atoi(optarg);
            if (startPackageSize >= 1 && startPackageSize<= 10000000000) {//10GiB max
            }
            else {
                printf("FAILURE \n-a: please enter vaild number for package size, which is not supposed to exceed 10GiB\n");
                exit(1);
            }
            break;
        case 'e':
            cutoff = atoi(optarg);
            if (cutoff >= 1 && cutoff <= 10000000000) {
            }
            else {
                printf("FAILURE \n-a: please enter vaild number for package size, which is not supposed to exceed 10GiB\n");
                exit(1);
            }
            break;
        case '?':
            fprintf (stderr,
                     "Unknown option character `\\x%x'.\n",
                     optopt);
        default:
            abort ();
    }


    printf ("sendmode = %d, startPackageSize = %ld, cutoff = %ld \n",
            sendmode, startPackageSize, cutoff);
}

size_t Totaldatasendcalc::getpackagesize(){
        return startPackageSize;
}
        
size_t Totaldatasendcalc::getcutoff(){
        return cutoff;
}

int const Totaldatasendcalc::getsendmode(){
    return sendmode;
}

size_t Totaldatasendcalc::getinnerRuntimeIterations(){
    iterations = empiricalfactor/packagesize_temp;
    return iterations;
}
    
size_t Totaldatasendcalc::getTotalDataSent(){
    size_t totalDataSent = iterations*packagesize_temp*sizeof(int);
    return totalDataSent;
}