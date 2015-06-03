#include "parameters.h"

Parameters::Parameters(int argc, char **argv){
    int opt;
    numberofwarmups = 100;
    multicore = 1;
    pipelinedepth = 8;
    factor = (10000000000);
    factor_fix = (1*(1<<20));
    buffersize = 34359738368;//4294967296; //2147483648;//!!!Attention in Bytes convert for pointer arithmetic
    pinningmode = 1;
    nmbr_commprocess = 4;
    startpackagesize = 1 << 2;
    endpackagesize = 1 << 24;
    packageSizeFactor = 2;
    
    //-------------------------------------------------------------------------------------------
    
    static struct option longopts[] = {
        { "help",               no_argument,              NULL,	     'h' },
        { "iterations",         required_argument,	     NULL,       'i' },
        { "start_package_size",    required_argument,	     NULL,	     'a' },
        { "cutoff",             required_argument,	     NULL,       'e' },
        { "package_size_factor",         required_argument,                 NULL,       'f' },
        { "buffer_size",            required_argument,	     NULL,       'b' },
        { "multicore",              required_argument,        NULL,       'm' },
        { "pipeline_depths",         required_argument,	     NULL,       'p' },
        { "warmups",                required_argument,	     NULL,       'w' },
        { "pinningmode",                required_argument,	     NULL,       'x' },
        { NULL,	     0,			     NULL,	     0 }
    };
    
    while ((opt = getopt_long (argc, argv, "hs:r:i:a:e:f:b:m:n:p:w:x:", longopts, NULL)) != -1)
        switch (opt)
    {
        case 'h':
            std::cout << "----------------------------------------\nWelcome to this MPI Benchmark program\n you may choose the following options\n -------------------------------------------"       << std::endl;
            std::cout << " --help                        -h       to view this help tutorial \n\n";
            std::cout << " --iterations                  -i       factor to determine number of inner runtime iterations in miollions \n (DEFAULT = "                        << factor << ")\n"                 << std::endl;
            std::cout << " --start_package_size          -a       start package size\n (DEFAULT = "                                                         << startpackagesize << ")\n"       << std::endl;
            std::cout << " --cutoff                      -e       endPackageSize, i.e. the maximum package size being send \n (DEFAULT = "                     << endpackagesize << ")\n"         << std::endl;
            std::cout << " --package_size_factor         -f       factor by which package size is increased \n (DEFAULT = "                                   << packageSizeFactor << ")\n"      << std::endl;
            std::cout << " --buffer_size                 -b       size of allocated buffer\n (DEFAULT = "                                                   << buffersize << ")\n"             << std::endl;
            std::cout << " --multicore                   -m       defines how many processes are initiated on a node \n (DEFAULT = "                           << multicore         << ")\n"      << std::endl;
            std::cout << " --nmbr-process                -n       defines how many processes are involed in communication througout bm \n (DEFAULT = "          << nmbr_commprocess        << ")\n"      << std::endl;
            std::cout << " --pipeline_depths             -p       pipeline depth (i.e. how many times a package is sent/received without waiting)\n (DEFAULT = " << pipelinedepth << ")\n"          << std::endl;
            std::cout << " --warmups                     -w       number of warmups (i.e. how many times a package is send/received in advance)\n (DEFAULT = "   << numberofwarmups << ")\n"        << std::endl;
            std::cout << " --pinningmode             -x       relevant for multicore ==1, 1 both processes on cpu0; 2 both processes on cpu1; 3 p0 on cpu0 & p1 on cpu1; 4 p0 on cpu1 & p1 on cpu0 \n (DEFAULT = "              << pinningmode << ") \n" << std::endl;
            
            exit(1);
        case 'i':
            factor = atoi(optarg);
            if (factor >= 1) {
                factor = 1000000*factor;
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
        case 'b':
            buffersize = atoll(optarg);
            if (buffersize > 0 && buffersize <= 500000000000) {
            }
            else {
                printf("ERROR -b: please enter vaild buffersize\n");
                exit(1);
            }
            break;
        case 'm':
            multicore = atoll(optarg);
            if (multicore > 0 && multicore <= 2) {
            }
            else {
                printf("ERROR -m: only valid for 1 and 2 thus far\n");
                exit(1);
            }
            break;
        case 'n':
            nmbr_commprocess = atoll(optarg);
            if (nmbr_commprocess > 1) {
            }
            else {
                printf("ERROR -n: must be at least two processes sending/receiving data\n");
                exit(1);
            }
            break;
        case 'p':
            pipelinedepth = atoi(optarg);
            
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
            pinningmode = atoi(optarg);
            if (!(pinningmode >= 1 && pinningmode <=4)) {
                printf("ERROR -t: only 1 (all processes on cpu0), 2 (all processes on cpu1), 3(sender on cpu0 & receiver on cpu1), 4(receiver on cpu0 and sender on cpu1) are possible \n");
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
                packagesizes.push_back(p);
        }
    }
    else{
        for (size_t p = startpackagesize; p >= endpackagesize; p = p/packageSizeFactor){
                packagesizes.push_back(p);
        }
    }
}




int Parameters::getsetflag(int rank){
    int commflag;
    if (rank < nmbr_commprocess){
        if (rank%2 == 0){
            commflag = 0;
            std::cout << "I am sender " << rank << std::endl;
        }
        else{
            commflag = 1;
            std::cout << "I am receiver " << rank << std::endl;
        }

    }
    else{
        commflag = 2;
        std::cout << " I am Observer " << rank << std::endl;
    }
    return commflag;
}


std::vector<size_t> Parameters::getinnerRuntimeIterations() {
    std::vector<size_t>innerRuntimeIterations;
    size_t iter;
    // inner iter for small packagesize constant because double the packagesize = double as fast
    std::cout << factor << " " << factor_fix << " " << numberofwarmups << std::endl;

    for(size_t pcksize = packagesizes.front(); pcksize <= packagesizes.back(); pcksize = pcksize*packageSizeFactor){
        if (pcksize <= 8000)  {
            iter = factor_fix;
        }
        else{
            iter = factor/(2*pcksize);
        }
        // inner iter for big package sizes which are all around 6GB/s
        iter = iter/nmbr_commprocess;

        if (iter <= 1){
            iter = 1;
            //innerRuntimeIterations = pipelinedepth + 1;
        }
        innerRuntimeIterations.push_back(iter);
    }
    return innerRuntimeIterations;
}