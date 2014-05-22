#include <iostream>
#include <mpi.h>
#include "parameters.h"
#include "results.h"
#include "buffer.h"
#include <unistd.h>
#include "tsc.h"

using namespace std;

int main (int argc, char *argv[]){
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    Parameters params(argc, argv);
    unsigned int pipelinedepth = params.getpipelinedepth();
    int numberofRootProcesses = params.getnumberofRootProcesses();
    int statisticaliteration = params.getStatisticalIterations();
    int numberofpackages = params.getNumberOfPackageSizes();
    
    Results results(rank, statisticaliteration, numberofpackages);
    Buffer buffer(size, rank, pipelinedepth, numberofRootProcesses, params.getBuffersize());
    double starttime, endtime;
    
    double singletime;
    
    TimeStampCounter timestamp;

    for (int m = 0; m <= statisticaliteration; m++){
        for (int z = 0; z < numberofpackages; z++){
            size_t packagesize = params.getPackageSizes().at(z);
            size_t packacount = packagesize/sizeof(int);
            size_t innerRuntimeIterations = params.getinnerRuntimeIterations(z, m);
            
            buffer.setloopvariables(packacount, innerRuntimeIterations);
            //cout << "package size: " << packagesize << ", iterations: " << innerRuntimeIterations << endl;
            
            if (rank < numberofRootProcesses){
                MPI_Barrier(MPI_COMM_WORLD);
                starttime = MPI_Wtime();
                timestamp.start();
                buffer.sendbuffer();
                MPI_Barrier(MPI_COMM_WORLD);
                endtime = MPI_Wtime();
                timestamp.stop();
                singletime=(double)timestamp.cycles();
            }
            
            else{
                MPI_Barrier(MPI_COMM_WORLD);
                starttime = MPI_Wtime();
                buffer.receivebuffer();
                MPI_Barrier(MPI_COMM_WORLD);
                endtime = MPI_Wtime();
            }
            
            
            if  (m != 0){
                
                //Write time-----------------------------------------------------------------
                cout << "# packagesize: " << packagesize << " processor cycles time[ms]" << "\n";
                cout << singletime << " " << singletime/2000 << " " << (endtime-starttime)/1000000 << "\n";
                //---------------------------------------------------------------------------
                results.setvectors((m-1), z, innerRuntimeIterations, packagesize, (endtime-starttime));
                //buffer.printsingletime((endtime-starttime));

            }
        }
    }
    
    //-----------------------------------Output-------------------------
    
    for (int i=0; i<size; i++) {
        if (rank == i){
            if(rank == 0){
                
                // Header
                time_t Zeitstempel;
                tm *nun;
                Zeitstempel = time(0);
                nun = localtime(&Zeitstempel);
                std::cout << "#" << nun->tm_mday << '.' << nun->tm_mon+1 << '.'<< nun->tm_year+1900 << " - " << nun->tm_hour << ':' << nun->tm_min << std::endl;
                
                cout << "# processes " << size << endl;
                cout << "#----------------------- SENDER ---------------------------" << endl;
                cout << "# totaldatasent repeats  packagesize time [us] std sendbandwidth [MB/s] std \n" << endl;
                results.calculate();
                cout << "\n\n" << endl;
                
                            }
            
            else if ( rank < numberofRootProcesses){
                results.calculate();
                cout << "\n\n" << endl;
            }
            
            else {
                cout << "#--- RECEIVER ----------------------------" << endl;
                cout << " number of receivers = " << size - numberofRootProcesses << endl;
                results.calculate();
                cout << "\n\n" << endl;
            }
            sleep(5);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        
    }

    MPI_Finalize();
    return 0;
}