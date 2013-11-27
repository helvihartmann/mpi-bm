// headerdatei that defines MPI Functions as a class
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/classmpi2.h
 27.10.2013
additional  Send recv */
#ifndef MPI_ROOT_
#define MPI_ROOT_
#include <mpi.h>
class Mpi{
private:
    int size;
    int rank;
    double timepoint;
public:
    //Konstruktoren
    //Mpi();
    // Elementfunktionen...blabla
    void init_it(int*, char***);
    
    int get_size();
    
    int get_rank();
    
    void performsend(void*, int, MPI_Datatype, int, int, MPI_Comm);

    
    void performrecv(void*, int, MPI_Datatype, int, int, MPI_Comm,MPI_Status*);
    
    void performalltoall(void*, int, MPI_Datatype,void*, int, MPI_Datatype,  MPI_Comm);
    
    double get_mpitime();
    
    /*char get_pname(char name, int &length){
        MPI_Get_processor_name(name, &length);
        return name;
    }*/
    
    void endmpi();
};
#endif