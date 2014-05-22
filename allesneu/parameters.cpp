#include "parameters.h"

Parameters::Parameters(int argc, char **argv){
    int opt;
    numberofwarmups = 20;
    pipelinedepth = 1;
    numberofRootProcesses = 1;
    statisticaliteration = 1;
    factor = 1<<20;
    buffersize = 1<<30;
    
    
    startpackagesize = 1 << 2;
    endpackagesize = 1 << 20;
    int packageSizeFactor = 2;
    
    //-------------------------------------------------------------------------------------------
    
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
            std::cout << "----------------------------------------\nWelcome to this MPI Benchmark program\n you may choose the following options\n -------------------------------------------"       << std::endl;
            std::cout << " --buffer_size                 -b       size of allocated buffer\n (DEFAULT = "                                                   << buffersize << ")\n"             << std::endl;
            std::cout << " --cutoff                      -e       endPackageSize, i.e. the maximum package size being send \n (DEFAULT = "                     << endpackagesize << ")\n"         << std::endl;
            std::cout << " --iterations                  -i       factor to determine number of inner runtime iterations\n (DEFAULT = "                        << factor << ")\n"                 << std::endl;
            std::cout << " --outer_statistical_iteations -o       statistical iterations of whole measurement\n (DEFAULT = "                                  << statisticaliteration << ")\n"   << std::endl;
            std::cout << " --help                        -h       to view this help tutorial \n\n";
            std::cout << " --package_size_factor         -f       factor by which package size is increased \n (DEFAULT = "                                   << packageSizeFactor << ")\n"      << std::endl;
            std::cout << " --pipeline_depths             -p       pipeline depth (i.e. how many times a package is sent/received without waiting)\n (DEFAULT = " << pipelinedepth << ")\n"          << std::endl;
            std::cout << " --warmups                     -w       number of warmups (i.e. how many times a package is send/received in advance)\n (DEFAULT = "   << numberofwarmups << ")\n"        << std::endl;
            std::cout << " --number_of_root_processes    -x       number of processes that send data to all others (min 1; max: 8) \n (DEFAULT = "              << numberofRootProcesses << ") \n" << std::endl;
            
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
            startpackagesize = 1<<atoi(optarg);
            
            if (startpackagesize >= 4 && startpackagesize <= 10000000000) {//10GiB max
            }
            else {
                printf("ERROR -a: please enter vaild number for package size, which is not supposed to exceed 10GiB\n");
                exit(1);
            }
            break;
        case 'e':
            endpackagesize = 1<<atoi(optarg);
            if (endpackagesize >= 1 && endpackagesize <= 50000000000) {
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
            statisticaliteration = atoi(optarg);
            if (statisticaliteration >=1 && statisticaliteration <= 1000) {
            }
            else {
                statisticaliteration=1000;
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
            pipelinedepth = atof(optarg);
            
            if (!(pipelinedepth > 0)) {
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

    //-------------------------------------------------------------------------------------------
    if (startpackagesize <= endpackagesize){
        for (size_t p = startpackagesize; p <= endpackagesize; p = p * packageSizeFactor){
                packageSizes.push_back(p);
        }
    }
    else{
        for (size_t p = startpackagesize; p >= endpackagesize; p = p/packageSizeFactor){
                packageSizes.push_back(p);
        }
    }
    
    std::cout<<"#start packagesize " << startpackagesize << ", inner iterations " << factor << ", end packagesize " << endpackagesize << ", statistical iterations " <<statisticaliteration << ", buffersize " << buffersize << ", pipeline depth " << pipelinedepth << ", number of warm ups " << numberofwarmups << ", number of senders " << numberofRootProcesses << std::endl;
}


size_t Parameters::getinnerRuntimeIterations(int z, int m) {
    size_t innerRuntimeIterations;
    
    if (m == 0){
        if (z == 0){
            std::cout << "#WARMUP" << std::endl;
        }
        innerRuntimeIterations = numberofwarmups;
    }
    else{
        if (z == 0){
            std::cout << "# " << m << ". iteration" << std::endl;
        }
        if (packageSizes.at(z) <= 1000)  {
            innerRuntimeIterations = factor;
        }
        else{
            innerRuntimeIterations = 50000000/packageSizes.at(z);
        }
        if (innerRuntimeIterations <= 5){
            innerRuntimeIterations = 5;
        }
    }
    
    
    return innerRuntimeIterations;
}