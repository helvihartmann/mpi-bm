#include "parameters.h"
/*08.08.2014 this class is responsible for reading all option parsed in the function call or othererwise setting the default values
 */
Parameters::Parameters(int argc, char **argv){
    int opt;
    numberofwarmups = 1000;
    multicore = 1;
    pipelinedepth = 8;
    numberofSenders = 1;
    statisticaliteration = 1;
    factor = (6000000000);
    factor_fix = (1*(1<<20));
    buffersize = 34359738368;//4294967296; //2147483648;//!!!Attention in Bytes convert for pointer arithmetic
    histcheck = 0;
    queue = 0;
    pinningmode = 1;
    
    barrelshiftingflag = on;
    
    startpackagesize = 1 << 2;
    endpackagesize = 1 << 20;
    int packageSizeFactor = 2;
    
    //-------------------------------------------------------------------------------------------
    
    static struct option longopts[] = {
        { "help",               no_argument,              NULL,	     'h' },
        { "iterations",         required_argument,	     NULL,       'i' },
        { "start_package_size",    required_argument,	     NULL,	     'a' },
        { "cutoff",             required_argument,	     NULL,       'e' },
        { "barrelshift",    required_argument,	     NULL,	     'r' },
        { "package_size_factor",         required_argument,                 NULL,       'f' },
        { "outer_statistical_iteations",         optional_argument,	      NULL,       'o' },
        { "buffer_size",            required_argument,	     NULL,       'b' },
        { "multicore",              required_argument,        NULL,       'm' },
        { "pipeline_depths",         required_argument,	     NULL,       'p' },
        { "queue",                  required_argument,	     NULL,       'q' },
        { "warmups",                required_argument,	     NULL,       'w' },
        { "number_senders",                required_argument,	     NULL,       's' },
        { "timedistribution",                required_argument,	     NULL,       't' },
        { "pinningmode",                required_argument,	     NULL,       'x' },
        { NULL,	     0,			     NULL,	     0 }
    };
    
    while ((opt = getopt_long (argc, argv, "hs:r:i:a:e:r:f:o:b:m:p:q:w:s:t:x:", longopts, NULL)) != -1)
        switch (opt)
    {
        case 'h':
            std::cout << "----------------------------------------\nWelcome to this MPI Benchmark program\n you may choose the following options\n -------------------------------------------"       << std::endl;
            std::cout << " --help                        -h       to view this help tutorial \n\n";
            std::cout << " --iterations                  -i       factor to determine number of inner runtime iterations in miollions \n (DEFAULT = "                        << factor << ")\n"                 << std::endl;
            std::cout << " --start_package_size          -a       start package size\n (DEFAULT = "                                                         << startpackagesize << ")\n"       << std::endl;
            std::cout << " --cutoff                      -e       endPackageSize, i.e. the maximum package size being send \n (DEFAULT = "                     << endpackagesize << ")\n"         << std::endl;
            std::cout << " --barrelshift                 -r       barrelshift \n (DEFAULT = "                     << barrelshiftingflag << ")\n"         << std::endl;
            std::cout << " --package_size_factor         -f       factor by which package size is increased \n (DEFAULT = "                                   << packageSizeFactor << ")\n"      << std::endl;
            std::cout << " --outer_statistical_iteations -o       statistical iterations of whole measurement\n (DEFAULT = "                                  << statisticaliteration << ")\n"   << std::endl;
            std::cout << " --buffer_size                 -b       size of allocated buffer\n (DEFAULT = "                                                   << buffersize << ")\n"             << std::endl;
            std::cout << " --multicore                   -m       defines how many processes are initiated on a node \n (DEFAULT = "                           << multicore         << ")\n"      << std::endl;
            std::cout << " --pipeline_depths             -p       pipeline depth (i.e. how many times a package is sent/received without waiting)\n (DEFAULT = " << pipelinedepth << ")\n"          << std::endl;
            std::cout << " --queue                       -q       one queue for all remoteranks (0) or independent queues (1) \n (DEFAULT = "              << queue << ") \n" << std::endl;
            std::cout << " --warmups                     -w       number of warmups (i.e. how many times a package is send/received in advance)\n (DEFAULT = "   << numberofwarmups << ")\n"        << std::endl;
            std::cout << " --number_senders             -s       number of processes that send data to all others (min 1; max: 8) \n (DEFAULT = "              << numberofSenders << ") \n" << std::endl;
            std::cout << " --timedistribution            -t       additional output of format <name>.hist containig timeinformation are printed (0=off, 1=on) \n (DEFAULT = "              << histcheck << ") \n" << std::endl;
            std::cout << " --pinningmode             -x       relevant for multicore ==1, 1 both processes on cpu0; 2 both processes on cpu1; 3 p0 on cpu0 & p1 on cpu1; 4 p0 on cpu1 & p1 on cpu0 \n (DEFAULT = "              << numberofSenders << ") \n" << std::endl;
            
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
        case 'r':
            barrelshiftingflag = static_cast<Flag>(atoi(optarg));
            if (atoi(optarg) >= 0 && atoi(optarg) <=1) {
            }
            else {
                printf("#INFO -r: 0 for on and 1 for off only \n");
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
        case 'p':
            pipelinedepth = atof(optarg);
            
            if (!(pipelinedepth > 0)) {
                printf("ERROR -p: please enter vaild number for number of calls (i.e. how many times a package is sent/received without waiting) \n");
                exit(1);
            }
            break;
        case 'q':
            queue = atoi(optarg);
            
            if (!(queue >= 0 && queue <= 1)) {
                printf("ERROR -q: queue can only be 0 for one queue for all remoteranks or 1 for one queue per remoterank \n");
                exit(1);
            }
            break;
        case 'w':
            numberofwarmups = atof(optarg);
            if (!(numberofwarmups >= 0)) {
                printf("ERROR -w: please enter vaild number for number of warmups (i.e. how many times a package is sent/received in advance) \n");
                exit(1);
            }
            break;
        case 's':
            numberofSenders = atof(optarg);
            if (!(numberofSenders > 0 && numberofSenders <=8)) {
                printf("ERROR -x: there are only 8 nodes, therefore only 8 possible root processes \n");
                exit(1);
            }
            break;
        case 't':
            histcheck = atoi(optarg);
            if (!(histcheck >= 0 && histcheck <=1)) {
                printf("ERROR -t: only 0 (off) and 1 (on) are possible \n");
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
                packageSizes.push_back(p);
        }
    }
    else{
        for (size_t p = startpackagesize; p >= endpackagesize; p = p/packageSizeFactor){
                packageSizes.push_back(p);
        }
    }
    
    std::cout<<"#start packagesize " << startpackagesize << ", inner iterations " << factor << ", end packagesize " << endpackagesize << ", statistical iterations " <<statisticaliteration << ", buffersize " << buffersize << ", pipeline depth " << pipelinedepth << ", natur of pipe" << queue <<  ", number of warm ups " << numberofwarmups << ", number of senders " << numberofSenders << ", multicore " << multicore << ", pinningmode " << pinningmode << ", barrelshift " << barrelshiftingflag << " (0 = on & 1 = off)" << std::endl;
}

std::vector<int> Parameters::getsetremoterankvec(unsigned int size_,unsigned int rank_){
    size = size_;
    rank = rank_;
    numberofReceivers = size - numberofSenders;
    switch (multicore) {
            case 1: {
                if(rank < numberofSenders){
                    
                    setflag(0,numberofReceivers);
                    sortlist(numberofSenders, size, (size + 1), 1);//exception out of scope, no implementation of barrelshift
                }
                else{
                    setflag(1,numberofSenders);
                    sortlist(0, numberofSenders, (size + 1), 1);
                }
            }
            break;
            case 2: {
                numberofSenders = (size/2) - 1;
                numberofReceivers = numberofSenders;
                //for senders
                if(rank%2 == 0 ){
                    
                    setflag(0,numberofReceivers);
                    //send to all odd ranked processes except the one on the same node (sender rank +1); either decreasing (sortlist) or ever process to a different port (barrelshifting)
                    switch (barrelshiftingflag) {
                        case on:
                            barrelshifting(3,1);
                            break;
                        case off:
                            sortlist(1, size, (rank + 1) , 2);
                            break;
                        default:
                            sortlist(1, size, (rank + 1) , 2);
                            break;
                    }
                }
                //for receivers
                else {
                    
                    setflag(1,numberofSenders);
                    //receive from all even ranked process except the one on the same node (receiver rank - 1)
                    switch (barrelshiftingflag) {
                        case on:
                            barrelshifting((size - 3),-1);//1 receives first from second last process
                            break;
                        case off:
                            sortlist(0, (size - 1), (rank - 1), 2);
                            break;
                            
                        default:
                            sortlist(0, (size - 1), (rank - 1), 2);
                            break;
                    }
                }
                for (unsigned int i=0; i<size; i++) {
                    if (rank == i){
                        for (unsigned int rank_index = 0; rank_index < remoterank_vec.size(); rank_index++){
                            std::cout << "my (" << rank << ") remoterank list is: " << remoterank_vec.at(rank_index) << std::endl;
                        }
                        
                    }
                }
               
            }
            break;
    }
    return remoterank_vec;
}

void Parameters::setflag(int commflag_, unsigned int numberofremoteranks_){
    numberofremoteranks = numberofremoteranks_;
    commflag = commflag_;
    if (commflag == 0){
        std::cout << "I am sender " << rank << std::endl;
    }
    else{
        std::cout << "I am receiver " << rank << std::endl;
    }
}

void Parameters::sortlist(unsigned int start, unsigned int end, unsigned int except, int increment){
    for (unsigned int remoterank_idx = start; remoterank_idx < end; remoterank_idx = (remoterank_idx + increment)){
        if (remoterank_idx != (except)){
            remoterank_vec.push_back(remoterank_idx);
        }
    }
}

void Parameters::barrelshifting(int start, int sign){
    for(unsigned int remoterank_idx = 0; remoterank_idx < numberofremoteranks; remoterank_idx++){
        unsigned int remoterank = (rank + start + (remoterank_idx * 2 * sign))%size;
        //0 should always start with sending to 3 and then add 2 in the next round
        remoterank_vec.push_back(remoterank);
    }
}



size_t Parameters::getinnerRuntimeIterations(int z) {
    size_t innerRuntimeIterations;
    
    if (factor > 100000000000){
        factor_fix = 50*factor_fix;
    }
    
    // inner iter for small packagesize constant because double the packagesize = double as fast
    if (packageSizes.at(z) <= 8000)  {
        innerRuntimeIterations = factor_fix;
    }
    else{
        innerRuntimeIterations = factor/packageSizes.at(z);
    }
    
    // inner iter for big package sizes which are all around 6GB/s
    if (numberofSenders < numberofReceivers){
        innerRuntimeIterations = innerRuntimeIterations * ((double)numberofSenders/(double)numberofReceivers);
    }
    else if (numberofSenders > numberofReceivers){
        innerRuntimeIterations = innerRuntimeIterations * ((double)numberofReceivers/(double)numberofSenders);
    }
    else if (numberofSenders == numberofReceivers){
        innerRuntimeIterations = innerRuntimeIterations/numberofReceivers;
    }
    
    if (innerRuntimeIterations <= 10){
        innerRuntimeIterations = 10;
    }
    
    return innerRuntimeIterations;
}