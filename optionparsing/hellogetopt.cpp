#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    int sendmode = 1; //send by default
    int startPackageSize = 2;
    //int index;
    int opt;
    
    opterr = 0;
    
    while ((opt = getopt (argc, argv, "m:p:")) != -1)
        switch (opt)
    {
        case 'm':
            sendmode = atoi(optarg);
            break;
        case 'p':
            startPackageSize = atoi(optarg);
            break;
        /*case '?':
            if (optopt == 'p')
                fprintf (stderr, "Option -%d requires an argument for starting size of package.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%d'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);*/
            return 1;
        default:
            abort ();
    }
    
    
    printf ("sendmode = %d, startPackageSize = %d\n",
            sendmode, startPackageSize);
    
    /*for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);
    return 0;*/
}