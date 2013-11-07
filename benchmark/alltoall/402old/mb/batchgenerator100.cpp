#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <cmath>
using namespace std;

/* file generates slurm_x?Byte.cpp files
 23.10.13*/

int main(){
    int i;
    double k,j;
    
            // for number of processes that exchange data
    for(i=2; i<=8; i++){
        k=1;
        stringstream streami;
        streami <<i;
        stringstream streamk;
        streamk <<k;
        string name="slurm_";
        name+=streamk.str().c_str();
        name+="00MB.cpp";
        fstream Slurm;
        //write output
        name = streami.str()+'/'+name;
        
        Slurm.open(name.c_str(), ios::out|ios::binary);
        Slurm<<"#!/bin/bash\n\n#SBATCH -N"<<i<<" -n"<<i;
        Slurm<<"\n#SBATCH -o 402_" << k <<"00MB.out"  <<"\n#SBATCH -J 402.cpp\n\nmpirun ~/firstests/402/402 "<< k<< "00000000\n\nexit 0";
        Slurm.close();

            }//for i
    return 0;
}


