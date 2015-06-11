#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <malloc.h>


using namespace std;

/*2015 Copyright Helvi Hartmann <hhartmann@fias.uni-frankfurt.de>

 */

void printtimestamp();
MPI_Comm setgroups(unsigned int numbercommprocesses, int rank);


int main (int argc, char **argv){
    
    // Header
    printtimestamp();
    


    int initialized;
    MPI_Comm boss_comm;
    
    MPI_Initialized(&initialized);
    if (!initialized) {
        MPI_Init(&argc,&argv);
        
        MPI_Comm_get_parent(&boss_comm);
        if (boss_comm != MPI_COMM_NULL) {
            // inside the spawned tasks
            int w_size,my_rank;
            MPI_Comm comm,intercom;
            
            MPI_Comm_size(MPI_COMM_WORLD,&w_size);
            MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
            printf("Slave task started, world size=%d my rank=%d\n",w_size,my_rank);
            
            printf("Slave task, merging the communicator...\n");
            MPI_Intercomm_merge(boss_comm,1,&comm);
            
            MPI_Comm_size(comm,&w_size);
            MPI_Comm_rank(comm,&my_rank);
            printf("Slave task after the merge, world size=%d my rank=%d\n",w_size,my_rank);
            
            printf("Slave, creating an intercom\n");
            
            int local_leader;
            
            MPI_Bcast(&local_leader,1,MPI_INT,0,comm);
            printf("Slave, local leader=%d\n",local_leader);
            MPI_Intercomm_create(comm,local_leader, // current comm , local leader
                                 MPI_COMM_WORLD,0, // peer comm , remote leader : significant only in the master
                                 100 , &intercom);
            
        } else {
            
            // the first task
            MPI_Comm intercomm_1,intercomm_2,comm_1,comm_2,full_comm1,full_comm2;
            MPI_Comm intercomm_full;
            
            printf("Master task: first spawn\n");
            MPI_Comm_dup(MPI_COMM_SELF,&comm_1);
            MPI_Comm_spawn("mpi-ft",MPI_ARGV_NULL,2,MPI_INFO_NULL,0,comm_1,&intercomm_1,(int*)MPI_ERRCODES_IGNORE);
            
            printf("Master task: second spawn\n");
            MPI_Comm_dup(MPI_COMM_SELF,&comm_2);
            MPI_Comm_spawn("mpi-ft",MPI_ARGV_NULL,2,MPI_INFO_NULL,0,comm_2,&intercomm_2,(int*)MPI_ERRCODES_IGNORE);
            
            printf("Master task: the two groups have been spawned\n");
            printf("Master task: merging the communicators...\n");
            
            printf("Master task: merging the first communicator...\n");
            MPI_Intercomm_merge(intercomm_1,0,&full_comm1);
            
            printf("Master task: merging the second communicator...\n");
            MPI_Intercomm_merge(intercomm_2,0,&full_comm2);
            
            printf("Master task: creating an intercom...\n");
            // Creates an intercom from two separated group communicator
            // Those two separeted communicators must have at least one task in common (the "leader")
            // whose rank must be known on each side.
            // Here the leader is the master task, whose rank is zero by construction
            
            int local_leader;
            
            local_leader=0; MPI_Bcast(&local_leader,1,MPI_INT,0,full_comm1);
            local_leader=1; MPI_Bcast(&local_leader,1,MPI_INT,0,full_comm2);
            
            MPI_Intercomm_create(full_comm1,0, // current comm , local leader
                                 full_comm2,1, // peer comm , remote leader
                                 100 , &intercomm_full);
            
            
        }
        
    }
    
    
    MPI_Finalize();
    printtimestamp();
    return 0;
}