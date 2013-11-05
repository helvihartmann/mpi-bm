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
    }
    
    int get_size(){
        MPI_Comm_size( MPI_COMM_WORLD, &size );
        return size;
    }
    
    int get_rank(){
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        return rank;
    }
};
//#endif