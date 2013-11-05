// headerdatei that defines MPI Functions as a class
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/classmpi1.h
 12.10.2013
 additional global Variables are replaced by a class Mpi*/
//#ifndef MPI_ROOT
//# define MPI_ROOT

class Mpi{
private:
    int size;
    int rank;
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
    
    void performalltoall(void *sendbuff, int sendcount, MPI_Datatype sendtype,void *recbuf, int recount, MPI_Datatype rectype,  MPI_Comm comm){
            MPI_Alltoall(	sendbuff,sendcount,sendtype,
                           recbuf, recount,rectype,
                           comm);
    }
    void endmpi(){
        MPI_Finalize();
    }
};
//#endif