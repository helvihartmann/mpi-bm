#include "parameters.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

/* pfad
27.11.2013
*/

void Parameters::readOptions(int argc, char **argv, int rank){
    int opt;
    factor = 100000*128;
    
    startPackageSize = 1<<2;
    endPackageSize = 1<<20;
    packageSizeFactor = 2;
    
    buffersize=1<<30;
    statisticaliterations=2;
    
    numberofcalls = 8;
    numberofwarmups = 10;
    
    numberofRootProcesses = 1;
    
    //int opterr = 0;
    
    static struct option longopts[] = {
        { "help",               no_argument,              NULL,	     'h' },
        { "iterations",         required_argument,	     NULL,       'i' },
        { "start_package_size",    required_argument,	     NULL,	     'a' },
        { "cutoff",             required_argument,	     NULL,       'e' },
        { "package_size_factor",         required_argument,                 NULL,       'f' },
        { "outer_statistical_iteations",         optional_argument,	      NULL,       'o' },
        { "buffer_size",            required_argument,	     NULL,       'b' },
        { "pipeline_depths",         required_argument,	     NULL,       'p' },
        { "warmups",                required_argument,	     NULL,       'w' },
        { "number_of_root_processes",                required_argument,	     NULL,       'x' },
        { NULL,	     0,			     NULL,	     0 }
    };
    
    while ((opt = getopt_long (argc, argv, "hs:r:i:a:e:f:o:b:p:w:x:", longopts, NULL)) != -1)
        switch (opt)
    {
        case 'h':
            if (rank == 0){
                std::cout<<"----------------------------------------\nWelcome to this MPI Benchmark program\n you may choose the following options\n -------------------------------------------" << std::endl;
                std::cout<<" --buffer_size                 -b       size of allocated buffer\n (DEFAULT = " << buffersize << ")\n" << std::endl;
                std::cout<<" --cutoff                      -e       endPackageSize, i.e. the maximum package size being send \n (DEFAULT = " << endPackageSize << ")\n" << std::endl;
                std::cout<<" --iterations                  -i       factor to determine number of inner runtime iterations\n (DEFAULT = "<< factor << ")\n" << std::endl;
                std::cout<<" --outer_statistical_iteations -o       statistical iterations of whole measurement\n (DEFAULT = "<< statisticaliterations << ")\n" << std::endl;
                std::cout << " --help                        -h       to view this help tutorial \n\n";
                std::cout<<" --package_size_factor         -f       factor by which package size is increased \n (DEFAULT = " << packageSizeFactor << ")\n" << std::endl;
                std::cout<<" --pipeline_depths             -p       pipeline depth (i.e. how many times a package is sent/received without waiting)\n (DEFAULT = " << numberofcalls << ")\n" << std::endl;
                std::cout<<" --warmups                     -w       number of warmups (i.e. how many times a package is send/received in advance)\n (DEFAULT = "<< numberofwarmups << ")\n" << std::endl;
                std::cout << " --number_of_root_processes   -x       number of processes that send data to all others (min 1; max: 8) \n (DEFAULT = " << numberofRootProcesses << ") \n" << std::endl;
            };

            exit(1);
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
            startPackageSize = 1<<atoi(optarg);
            
            if (startPackageSize >= 4 && startPackageSize <= 10000000000) {//10GiB max
            }
            else {
                printf("ERROR -a: please enter vaild number for package size, which is not supposed to exceed 10GiB\n");
                exit(1);
            }
            break;
       case 'e':
            endPackageSize = 1<<atoi(optarg);
            if (endPackageSize >= 1 && endPackageSize <= 50000000000) {
            }
            else {
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
        case 'p':
            numberofcalls = atof(optarg);
            
            if (!(numberofcalls > 0)) {
                printf("ERROR -p: please enter vaild number for number of calls (i.e. how many times a package is sent/received without waiting) \n");
                exit(1);
            }
            break;
        case 'w':
            numberofwarmups = atof(optarg);
            if (!(numberofwarmups > 0)) {
                printf("ERROR -w: please enter vaild number for number of warmups (i.e. how many times a package is sent/received in advance) \n");
                exit(1);
            }
            break;
        case 'x':
            numberofRootProcesses = atof(optarg);
            if (!(numberofRootProcesses > 0 && numberofRootProcesses <=8)) {
                printf("ERROR -x: there are only 8 nodes, therefore only 8 possible root processes \n");
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
    std::cout<<"#start packagesize " << startPackageSize << ", inner iterations " << factor << ", end packagesize " << endPackageSize << ", statistical iterations " <<statisticaliterations << ", buffersize " << buffersize << ", pipeline depth " << numberofcalls << ", number of warm ups " << numberofwarmups << ", number of senders " << numberofRootProcesses << std::endl;
    
    if (startPackageSize <= endPackageSize)
        for (size_t p = startPackageSize; p <= endPackageSize; p = p * packageSizeFactor)
            packageSizes.push_back(p);
    else
        for (size_t p = startPackageSize; p >= endPackageSize; p = p/packageSizeFactor)
            packageSizes.push_back(p);
}

size_t Parameters::getinnerRuntimeIterations(size_t packageSize) {
    size_t iterations;
    
    if (factor==1) {
        iterations=1;
    }
    else {
        iterations = factor/ packageSize;
            if (iterations<=20) {
            iterations=20;
        }
    }
    
    /*if (iterations * packageSize > 4000000000){
        iterations = 10;
    }*/
   
    return iterations;
}
