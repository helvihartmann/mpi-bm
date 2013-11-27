#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <cmath>
using namespace std;

/* file generates slurm_x?Byte.cpp files
 07.11.13*/

int main(){
    int i;
    double k,j;
    int version=523;
    stringstream streamv;
    streamv <<version;
    
    // this loop is for creating the amount of data referred to as k Byte
    int l=20;
    
    // for number of processes that exchange data
    i=2;
        
    
//long int iterations2 = 5000000;
    int m = 0;
    for(j=7; j<l;j++){
        m++;
        stringstream streami, streamm, streamk;
        streami <<i;
        streamm << m;
        double basis = 2;
        k=pow(basis,j);
        long double factor = 500/k;
        size_t iterations2 = round(5000000*factor);
        streamk <<k;
        
        string name="slurm_";
        name+=streamk.str().c_str();
        name+="B.cpp";
        
        fstream Slurm;
                //write output
        name = streamv.str()+'/'+name; //cuz no xtra folders generated
        Slurm.open(name.c_str(), ios::out|ios::binary);
        Slurm<<"#!/bin/bash\n\n#SBATCH -N"<<i<<" -n"<<i;
        Slurm<<"\n#SBATCH -o "<<version<<"_"<< m <<"_"<< k <<"B.out"  <<"\n#SBATCH -J "<<version<<".cpp\n\nmpirun --mca btl_openib_warn_no_device_params_found 0 "<<version<<" "<< k<<" "<<iterations2<<"\n\nexit 0";
        Slurm.close();
//iterations2 = iterations2/2;
    }//for j
       // }// i
    return 0;
}
