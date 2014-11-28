#include <numa.h>
#include <iostream>
#include <vector>
#include <mpi.h>

/*19.08.2014 pins processes to specific CPU's
 17.10 altered to file containing all functions that dont need to be in classes
 */


MPI_Comm setgroups(int numbercommprocesses,int rank){
    int ranks[numbercommprocesses];
    
    for (int i=0; i<numbercommprocesses; i++){
        ranks[i]=i;
    }
    MPI_Group worldgroup, communicatorsgroup;
    MPI_Comm communicators_comm;
    //get a handle to the group which contains all process: worldgroup
    MPI_Comm_group(MPI_COMM_WORLD, &worldgroup);
    //creates a group (communicatorsgroup) that consists of the n processes in worldgroup with ranks in vector ranks
    MPI_Group_incl(worldgroup, numbercommprocesses, ranks, &communicatorsgroup);
    MPI_Comm_create(MPI_COMM_WORLD,communicatorsgroup, &communicators_comm);
    
    //get rank of processes in each Communicator
    int communicatorsgrouprank;
    MPI_Group_rank(communicatorsgroup, &communicatorsgrouprank);
    
    std::cout << "I am " << rank << " and my rank in the newgroup is " << communicatorsgrouprank << std::endl;
    
    return communicators_comm;
}


void pinning(int commflag, int pinningmode){
    struct bitmask* nodemask = numa_allocate_cpumask();

    switch (pinningmode) {
        case 1:{
            numa_bitmask_setbit(nodemask, 0);
            std::cout << "pinning all processes to numa_node 0" << std::endl;
            numa_bind(nodemask);
            numa_set_membind(nodemask);
        }
            break;
        case 2:{
            numa_bitmask_setbit(nodemask, 1);
            std::cout << "pinning sender and receiver to numa_node 1" << std::endl;
            numa_bind(nodemask);
            numa_set_membind(nodemask);
        }
            break;
        case 3:{
            if (commflag == 0){
                numa_bitmask_setbit(nodemask, 0);
                std::cout << "pinning sender to numa_node 0" << std::endl;
                numa_bind(nodemask);
                numa_set_membind(nodemask);
            }
            else{
                numa_bitmask_setbit(nodemask, 1);
                std::cout << "pinning receiver to numa_node 1" << std::endl;
                numa_bind(nodemask);
                numa_set_membind(nodemask);
            }
        }
            break;
        case 4:{
            if (commflag == 0){
                numa_bitmask_setbit(nodemask, 1);
                std::cout << "pinning sender to numa_node 1" << std::endl;
                numa_bind(nodemask);
                numa_set_membind(nodemask);
            }
            else{
                numa_bitmask_setbit(nodemask, 0);
                std::cout << "pinning receiver to numa_node 0" << std::endl;
                numa_bind(nodemask);
                numa_set_membind(nodemask);
            }
        }
            break;
        default:
            break;
    }
}


void printtimestamp(){
    time_t Zeitstempel;
    tm *nun;
    Zeitstempel = time(0);
    nun = localtime(&Zeitstempel);
    std::cout << "#" << nun->tm_mday << '.' << nun->tm_mon+1 << '.'<< nun->tm_year+1900 << " - " << nun->tm_hour << ':' << nun->tm_min << ':' << nun->tm_sec << std::endl;
}