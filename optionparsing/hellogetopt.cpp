#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    int sendmode = 1; //send by default
    size_t startPackageSize = 2;
    size_t cutoff = 1000;
    int opt;
    
    opterr = 0;
    
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
            if (cutoff >= 1 && cutoff <= 10) {
                
            }
            else {
                printf("FAILURE \n-e: please enter vaild number for package size, which is not supposed to exceed 10GiB\n");
                exit(1);
            }
            break;
        case '?':
            fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
    }
    
    
    printf ("sendmode = %d, startPackageSize = %ld, cutoff = %ld \n",
            sendmode, startPackageSize, cutoff);
}