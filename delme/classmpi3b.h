// headerdatei that defines MPI Functions as a class
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/classmpi2.h
 07.11.2013
adopted class to perform Ssen/Srecvadditional  Send recv 
 ssend mpi will not return until matching receive posted */
#ifndef MPI_ROOT_
#define MPI_ROOT_

class Mpi{
private:
    int size;
    int rank;
    double timepoint;
public:
    //Konstruktoren
    //Mpi();
    // Elementfunktionen...blabla
    void init_it(int  *argc, char ***argv) {
        MPI_Init(argc,argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size );
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    }
    
    int get_size(){
        return size;
    }
    
    int get_rank(){
        return rank;
    }
    
    void performsend(void *sendbuff, int sendcount, MPI_Datatype sendtype, int destination, int tag, MPI_Comm comm){
        MPI_Bsend(sendbuff,sendcount,sendtype, destination, tag, comm);
    }

    
    void performrecv(void *recbuf, int recount, MPI_Datatype rectype, int source, int tag, MPI_Comm comm,MPI_Status *status){
        MPI_Recv(recbuf, recount,rectype, source, tag, comm, status);
    }
    
    void performalltoall(void *sendbuff, int sendcount, MPI_Datatype sendtype,void *recbuf, int recount, MPI_Datatype rectype,  MPI_Comm comm){
            MPI_Alltoall(	sendbuff,sendcount,sendtype,
                           recbuf, recount,rectype,
                           comm);
    }
    
    double get_mpitime(){
        timepoint = MPI_Wtime();
        return timepoint;
    }
    
    /*char get_pname(char name, int &length){
        MPI_Get_processor_name(name, &length);
        return name;
    }*/
    
    void endmpi(){
        MPI_Finalize();
    }
};
#endif