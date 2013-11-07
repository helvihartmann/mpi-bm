// headerdatei that defines MPI Functions as a class
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/classmpi2.h
 17.10.2013
additional  WTime*/
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