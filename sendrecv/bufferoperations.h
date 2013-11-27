#include "mpi.h"
/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
 14.11.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes even more than 1GB
 This version iterates over the packege size to assure for every run the same conditions on the fles
 and prints everything into 503.out*/

class Bufferoperations{
private:
    int *scounts,*rcounts;
    size_t packagesize_temp;
    size_t timepackageissend;
    Mpi mpi1;
public:
    
    void setPackagesizeTmp(size_t p){
        packagesize_temp = p;
    }
    
    void setIterations2(size_t iterations2){
        timepackageissend = iterations2;
    }
    
    void allocateBuffer(){
        scounts= new int [packagesize_temp];
        rcounts= new int [packagesize_temp];
    }
    
    void initalizeBuffer(){
        for(size_t i=0; i<packagesize_temp; i++){
            scounts[i]=1;

        }
    }
    
    void sendBuffer(){
        for(int j=0; j<timepackageissend; j++){
            //std::cout << "scounts  "<< *scounts<<"\n";
            mpi1.performsend(scounts,packagesize_temp,MPI_INT,1,j,MPI_COMM_WORLD);
        }
    }
    
    void recvBuffer(){
        for(int j=0; j<timepackageissend; j++){
            mpi1.performrecv(rcounts,packagesize_temp,MPI_INT,0,j,
                             MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
        //std::cout << "rcounts  "<< *rcounts<<"\n";
    }
    
    void checkBuffer(size_t *everythingcorrect_check){
        //std::cout << "rcounts oben "<< *rcounts<<"\n \n \n";
        for(size_t i=0;i<packagesize_temp;i++){
            if(rcounts[i]!=1){
                everythingcorrect_check++;
            }
        }
    }
    
    void freeBuffer(){
        delete [] scounts;
        delete [] rcounts;
    }
    
};
