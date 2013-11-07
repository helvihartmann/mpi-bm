// headerdatei that defines MPI Functions as a class
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/classmpi.h
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
    Mpi();
    // Elementfunktionen...blabla
    int get_size();
    int get_rank();
    void init_it(int  *argc, char ***argv);
};
//#endif