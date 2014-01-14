#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int
main (int argc, char **argv)
{
    int sendmode = 1; //send by default
    size_t startPackageSize = 2;
    size_t cutoff = 1000;
    int statisticaliterations=1000;
    size_t startiteration=1000000*128;
    int opt;
    
    opterr = 0;
    
    while ((opt = getopt (argc, argv, "hm:a:i:e:o:")) != -1)
        switch (opt)
    {
        case 'h':
            cout<<"----------------------------------------\nWelcome to this MPI Benchmark program\n you may choose the following options\n -------------------------------------------\n";
            cout<<" -m      sendmode: 1 = MPI_Send, 2 = MPI_Ssend, 3 = MPI_Bsend\n \t(DEFAULT = 1)\n";
            cout<<" -a      start package size of data that is send between two processes and which will be doubled until cutoff\n\t(DEFAULT = 2B)\n";
            return 0;
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
            if (startiteration >=1 && startiteration<= 10000000000) {
            }
            else {
                startiteration = 1000000*128;
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
    
    
    printf ("sendmode = %d, startPackageSize = %ld, start iterations = %ld cutoff = %ld, statistical iterations = %d \n",
            sendmode, startPackageSize, startiteration, cutoff, statisticaliterations);
}