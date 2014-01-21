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
    size_t factor = 100000*128;
    startiteration = factor;
    cutoff = 1000;
    startPackageSize = 2;
    sendmode = 1;
    statisticaliterations=10;
    
    //int opterr = 0;

    while ((opt = getopt (argc, argv, "hm:a:i:e:o:")) != -1)
        switch (opt)
    {
       /* case 'h':
            sendmode = atoi(optarg);
            if (sendmode >= 1 && sendmode <= 3) {
                //zwischen 1 und 3
            }
            else {
                printf("ERROR -m: your options are 1 send; 2 Ssend, 3 Bsend\n");
                exit(1);
            }
            break;*/
        case 'm':
            sendmode = atoi(optarg);
            if (sendmode >= 1 && sendmode <= 3) {
                //zwischen 1 und 3
            }
            else {
                printf("ERROR -m: your options are 1 send; 2 Ssend, 3 Bsend\n");
                exit(1);
            }
            break;
        case 'a':
            startPackageSize = atoi(optarg);
            
            if (startPackageSize >= 1 && startPackageSize<= 10000000000) {//10GiB max
            }
            else {
                printf("ERROR -a: please enter vaild number for package size, which is not supposed to exceed 10GiB\n");
                exit(1);
            }
            break;
        case 'i':
            startiteration = atoi(optarg);
            if (startiteration ==1) {
            }
            else {
                startiteration = factor;
                printf("#INFO -i: you can only fix iteration to 1 (-i 1) for debugging. Default value is used: 128*20mio \n");
            }
            break;
        case 'e':
            cutoff = atoi(optarg);
            if (cutoff >= 1 && cutoff <= 50000000000) {
            }
            else {
                cutoff = startiteration;
                printf("#INFO -e: max package size was set to 50GB \n");
            }
            break;
        case 'o':
            statisticaliterations = atoi(optarg);
            if (statisticaliterations >=1 && statisticaliterations <= 1000) {
            }
            else {
                statisticaliterations=1000;
                printf("#INFO -o: statistical iterations were limited to 1000 \n");
            }
            break;
        case '?':
            fprintf (stderr,
                     "ERROR: Unknown option character `\\x%x'.\n",
                     optopt);
        default:
            abort ();
    }
    printf ("#sendmode = %d, startPackageSize = %ld, start iterations = %ld cutoff = %ld, statistical iterations = %d \n",
            sendmode, startPackageSize, startiteration, cutoff, statisticaliterations);

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


size_t Totaldatasendcalc::getinnerRuntimeIterations(int z){
    if(startiteration==1){
        iterations=1;
    }
    else {
        if (packagesize_temp*sizeof(int) < 128){
            iterations = 3200000;
        }
        else if (packagesize_temp*sizeof(int) >= 128 & packagesize_temp*sizeof(int) < 1024){
            iterations = 100000;
        }
        else if (packagesize_temp*sizeof(int) >= 1024 & packagesize_temp*sizeof(int) < 32768){
            iterations = 50000;
        }
        else if (packagesize_temp*sizeof(int) >=32768 & packagesize_temp*sizeof(int) < 1048576){
            iterations = 10000;
        }
        else if (packagesize_temp*sizeof(int) >=1048576 & packagesize_temp*sizeof(int) < 4194304){
            iterations = 2000;
        }
        else if (packagesize_temp*sizeof(int) >= 4194304 & packagesize_temp*sizeof(int) <8388608){
            iterations = 400;
        }
        else if (packagesize_temp*sizeof(int) >= 8388608){
            iterations = (400*(8388608/sizeof(int)))/packagesize_temp;
        }
        /*else if (packagesize_temp*sizeof(int) > 16777216 & packagesize_temp*sizeof(int) <= 33554432){
            iterations = 100;
        }*/
        
        /*iterations = (startiteration/packagesize_temp);
        if(iterations<=200){
            iterations=200;
        }*/
    }
    
    if (iterations*packagesize_temp*sizeof(int) > 45000000000){
        iterations = 10;
    }
   
    if (iterations < 1){
        iterations = 1;
    }
    
    return iterations;
}
    
size_t Totaldatasendcalc::getTotalDataSent(){
    size_t totalDataSent = iterations*packagesize_temp*sizeof(int);
    return totalDataSent;
}
