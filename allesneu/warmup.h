#include <iostream>
#include "buffer.h"

class Warmup{
public:
    Warmup(Buffer *buffer,int commflag,size_t numberofwarmups, int rank, unsigned int numberofSenders, unsigned int numberofReceivers);
};

Warmup::Warmup(Buffer *buffer, int commflag, size_t numberofwarmups, int rank, unsigned int numberofSenders, unsigned int numberofReceivers){
    if (rank == 0){
        std::cout << "#warmup " << std::endl;
    }
    
    if (numberofSenders == numberofReceivers){
        numberofwarmups = numberofwarmups/numberofReceivers;
    }
    
    for (size_t packagecount = 1; packagecount < 1<<24; packagecount = packagecount*2){
        buffer->setloopvariables(packagecount, numberofwarmups);
        
        
        if (commflag == 0){
            double starttime = MPI_Wtime();
            buffer->sendbuffer();
            double endtime = MPI_Wtime();
            std::cout << numberofwarmups*packagecount*sizeof(int) << " " << numberofwarmups << " " << packagecount*sizeof(int) << " " << endtime - starttime << " - " << (numberofwarmups*packagecount*sizeof(int))/((endtime - starttime)*1000000) << " - " << rank << std::endl;
        }
        else{
            buffer->receivebuffer();
        }
        
    }
    std::cout << " " << std::endl;
}
