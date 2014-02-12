#include "parameters.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cmath>

/* pfad
27.11.2013
*/

void Parameters::readOptions(int argc, char **argv){
    int opt;
    factor = 100000*128;
    
    startPackageSize = 4;
    endPackageSize = 1000;
    packageSizeFactor = 2.0;
    
    buffersize=pow(2,35);//34GB
    sendmode = 1;
    recvmode = 1;
    statisticaliterations=10;
    
    //int opterr = 0;

    while ((opt = getopt (argc, argv, "hs:r:a:i:e:o:f:b:")) != -1)
        switch (opt)
    {
        case 'h':
            std::cout<<"----------------------------------------\nWelcome to this MPI Benchmark program\n you may choose the following options\n -------------------------------------------\n";
            std::cout<<" -s      sendmode: 1 = MPI_Send, 2 = MPI_Ssend, 3 = MPI_Bsend, 4 = MPI_Send_Init, 5 = MPI_Isend \n \t(DEFAULT = 1)\n";
            std::cout<<" -r      sendmode: 1 = 2 = 3 = MPI_Recv, 4 = MPI_Recv_Init, 5 = MPI_Irecv \n \t(DEFAULT = 1)\n";
            std::cout<<" -a      start package size of data that is send between two processes and which will be doubled until endPackageSize\n\t(DEFAULT = 2B)\n";
            std::cout<<" -e      endPackageSize, i.e. the maximum package sized that is being analyzed\n";
            std::cout<<" -i      choose -i 1 for only one iteration for every package size to be fast\n";
            std::cout<<" -o      statisticaliterations\n";
            std::cout<<" -f      factor to determine number of inner runtime iterations\n";
            std::cout<<" -b      size of allocated buffer\n";
            exit(1);
        case 's':
            sendmode = atoi(optarg);
            if (sendmode >= 1 && sendmode <= 5) {
                //zwischen 1 und 3
            }
            else {
                printf("ERROR -s: your options are 1 send; 2 Ssend, 3 Bsend, 4 send_init and 5 Isend \n");
                exit(1);
            }
            break;
        case 'r':
            recvmode = atoi(optarg);
            if (recvmode >= 1 && recvmode <= 5) {
                //zwischen 1 und 3
            }
            else {
                printf("ERROR -r: your options are 1,2,3 recv, 4 recv_init and 5 Irecv\n");
                exit(1);
            }
            break;
        case 'i':
            factor = atoi(optarg);
            if (factor >= 1) {
            }
            else {
                printf("ERROR -i: please enter vaild iteration factor\n");
                exit(1);
            }
            break;
        case 'a':
            startPackageSize = atoi(optarg);
            
            if (startPackageSize >= 4 && startPackageSize <= 10000000000) {//10GiB max
            }
            else {
                printf("ERROR -a: please enter vaild number for package size, which is not supposed to exceed 10GiB\n");
                exit(1);
            }
            break;
       case 'e':
            endPackageSize = atoi(optarg);
            if (endPackageSize >= 1 && endPackageSize <= 50000000000) {
            }
            else {
                endPackageSize = factor;
                printf("#INFO -e: max package size was set to 50GB \n");
            }
            break;
        case 'f':
            packageSizeFactor = atof(optarg);
            
            if (!(packageSizeFactor > 0)) {
                printf("ERROR -f: please enter vaild number for step factor\n");
                exit(1);
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
        case 'b':
            buffersize = atoi(optarg);
            if (buffersize > 0 && buffersize <= 50000000000) {
            }
            else {
                printf("ERROR -b: please enter vaild buffersize\n");
                exit(1);
            }
            break;
        case '?':
            fprintf (stderr,
                     "ERROR: Unknown option character `\\x%x'.\n",
                     optopt);
        default:
            abort ();
    }
    std::cout<<"# sendmode" << sendmode << " ,receivemode " << recvmode << ", start packagesize " << startPackageSize << ", factor " << factor << ", end packagesize " << endPackageSize << ", statistical iterations " <<statisticaliterations << ", buffersize " << buffersize <<std::endl;
    
    if (startPackageSize <= endPackageSize)
        for (size_t p = startPackageSize; p <= endPackageSize; p = p * packageSizeFactor)
            packageSizes.push_back(p);
    else
        for (size_t p = startPackageSize; p >= endPackageSize; p = p * packageSizeFactor)
            packageSizes.push_back(p);
}

size_t Parameters::getinnerRuntimeIterations(size_t packageSize) {
    size_t iterations;
    
    if (factor==1) {
        iterations=1;
    }
    else {
        iterations = factor/ packageSize;
            if (iterations<=10) {
            iterations=10;
        }
    }
    
    /*if (iterations * packageSize > 4000000000){
        iterations = 10;
    }*/
   
    return iterations;
}
