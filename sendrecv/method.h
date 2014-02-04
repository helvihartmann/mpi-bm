#ifndef BUFFER_H
#define BUFFER_H

#include <mpi.h>
#include "restults.h"
#include "buffer.h"
#include "tsc.h"
/* 04.02.14 The send method used to test bandwidth is specified here
 (1) pingpong communication between the processes: p0 sends that and gets it back from p1*/



Method(int remoteRank, Results *results, Buffer *buffer){

    size_t p = params.getPackageSizes().at(z);
    size_t innerRuntimeIterations = params.getinnerRuntimeIterations(p);
    double starttime, endtime;
    buffer->setloopvariables(p, innerRuntimeIterations, 1);
                    
    starttime =MPI_Wtime();
                    
    //double starttime_inner, endtime_inner;
    timeStampCounter.start();
    for(size_t j=0; j<innerRuntimeIterations; j++){
        buffer->sendBuffer(j);
        buffer->recvBuffer(j);
    }
    endtime = MPI_Wtime();
    timeStampCounter.stop();
    cycle = timeStampCounter.cycles();
    cout <<" timestampcounter: " << timeStampCounter.cycles() << " cycle/3.6GHz: " << cycle/3600 << " us" << endl;
                    
    if(m!=0){
        results->settime((m-1),z,((endtime-starttime)/2));
    }
}
