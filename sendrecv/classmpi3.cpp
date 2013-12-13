#include "classmpi3.h"
/* PFAD
 27.11.2013
additional  Send recv */

void Mpi::init_it(int  *argc, char ***argv) {
        MPI_Init(argc,argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size );
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}
    
int Mpi::get_size(){
        return size;
}
    
int Mpi::get_rank(){
        return rank;
}
    
void Mpi::performsend(void *sendbuff, int sendcount, MPI_Datatype sendtype, int destination, int tag, MPI_Comm comm, const int* sendmode){
    
    switch (*sendmode) {
        case 1:
            MPI_Send(sendbuff,sendcount,sendtype, destination, tag, comm);
            break;
        case 2:
            MPI_Ssend(sendbuff,sendcount,sendtype, destination, tag, comm);
            break;
        case 3:
            MPI_Bsend(sendbuff,sendcount,sendtype, destination, tag, comm);
            break;
        default:
            MPI_Send(sendbuff,sendcount,sendtype, destination, tag, comm);
            break;
    }
}

    
void Mpi::performrecv(void *recbuf, int recount, MPI_Datatype rectype, int source, int tag, MPI_Comm comm,MPI_Status *status){
        MPI_Recv(recbuf, recount,rectype, source, tag, comm, status);
}
    
void Mpi::performalltoall(void *sendbuff, int sendcount, MPI_Datatype sendtype,void *recbuf, int recount, MPI_Datatype rectype,  MPI_Comm comm){
            MPI_Alltoall(	sendbuff,sendcount,sendtype,
                           recbuf, recount,rectype,
                           comm);
}
    
double Mpi::get_mpitime(){
        timepoint = MPI_Wtime();
        return timepoint;
}
    
    /*char get_pname(char name, int &length){
        MPI_Get_processor_name(name, &length);
        return name;
    }*/
    
void Mpi::endmpi(){
        MPI_Finalize();
}