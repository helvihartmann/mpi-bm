#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <unistd.h>
using namespace std;

/* fles:~/benchmark/sendrecv 27.11.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes as specified in console
 This version iterates over the packege size to assure for every run the same conditions on the fles
 and prints everything into sendrecv.out when runned via 
 > sbatch slurm_test.in*/

int main(int argc,char *argv[]){
    
    /*--------------------------------------start MPI-----------------------------*/
    //cout<<"\n hallo \n";
    int size, rank;
    int length;
    char name[MPI_MAX_PROCESSOR_NAME];
 
    //sleep(15);
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &length); //not yet handled in class
    cout<<"# Prozess " << rank << " von " <<size<<" on "<< name<<" \n";

    size_t buffersize = 50000000000/sizeof(int);
    int* buffer= new int [buffersize];//von VoLi angewiesene Größe
    std::cout<<buffersize*sizeof(int)<<"B allocated \n";
    
    size_t packagesize =1000000;//1MB
    size_t innerRuntimeIterations=1;
    
    for(int j=0; j<innerRuntimeIterations; j++){
               //Process 0 sends the data
        if (rank == 0) {
            
            /*for(int i=0; i<ncounts; i++){
                scounts[i]=1;
            }*/
            
            double starttime = MPI_Wtime();
            //cout<<"# start sending \n";
            MPI_Send(buffer,packagesize,MPI_INT,1, packagesize+1, MPI_COMM_WORLD);
            double endtime = MPI_Wtime();
            cout<<"sending took "<<endtime-starttime<<"\n";

        }
    
        //Process 1 receives the data
        else if (rank == 1) {
            int rcounts;
            //cout<<"# start receiving\n";
            MPI_Recv(buffer, packagesize,MPI_INT, 0, packagesize+1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //cout<<"# end receiving\n";

            
            /*size_t everythingcorrect_check=0;
            for(int i=0; i<ncounts; i++){
                if(rcounts[i]!=0){
                    everythingcorrect_check++;
                }
            }*/

        }//else if
    }//for j~innerRuntimeIterations
    delete [] buffer;
    //free(scounts);
    MPI_Finalize();
}