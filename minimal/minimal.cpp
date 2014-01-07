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

    /*int *scounts,*rcounts;
    int ncounts = 2;
    
    scounts = (int*) calloc(ncounts,sizeof(int));
    cout<<"#scounts"<<*scounts<<"\n";
    //scounts= new int [ncounts];
    rcounts= new int [ncounts];*/
    
    size_t innerRuntimeIterations=1;
    //double starttime_send, endtime_send, starttime_recv, endtime_recv;
    //double recvtime[outerStatisticalIterations], sendtime[outerStatisticalIterations];
    
    for(int j=0; j<innerRuntimeIterations; j++){
               //Process 0 sends the data
        if (rank == 0) {
            
            /*for(int i=0; i<ncounts; i++){
                scounts[i]=1;
            }*/
            int scounts =1;
            //cout<<"# start sending \n";
            MPI_Send(&scounts,1,MPI_INT,1, 2, MPI_COMM_WORLD);
            
            //cout<<"# end sending\n";

        }
    
        //Process 1 receives the data
        else if (rank == 1) {
            int rcounts;
            //cout<<"# start receiving\n";
            MPI_Recv(&rcounts, 1,MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //cout<<"# end receiving\n";

            
            /*size_t everythingcorrect_check=0;
            for(int i=0; i<ncounts; i++){
                if(rcounts[i]!=0){
                    everythingcorrect_check++;
                }
            }*/

        }//else if
    }//for j~innerRuntimeIterations
    //free(scounts);
    MPI_Finalize();
}