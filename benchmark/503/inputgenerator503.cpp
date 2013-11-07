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
    int version=503;
    
    // this loop is for creating the amount of data referred to as k Byte 
    int l=20;
    //for(i=2; i<=8; i++){
        //l--;
    i=2;
        
            // for number of processes that exchange data
    long int iterations2 = 5000000;
    for(j=7; j<l;j++){
        //i=3; // test only for 2 Processes talking
        stringstream streami;
        streami <<i;
                
                //creates folders sorted by number of processes
                /*if(j==1){
                    mkdir(streami.str().c_str(),0770);
                }*/
        double basis = 2;
        k=pow(basis,j);
        stringstream streamk;
        streamk <<k;
        string name="slurm_";
        name+=streamk.str().c_str();
        name+="B.cpp";
        
        fstream Slurm;
                //write output
                //name = streami.str()+'/'+name; //cuz no xtra folders generated
        Slurm.open(name.c_str(), ios::out|ios::binary);
        Slurm<<"#!/bin/bash\n\n#SBATCH -N"<<i<<" -n"<<i;
        Slurm<<"\n#SBATCH -o "<<version<<"_"<< k <<"B.out"  <<"\n#SBATCH -J "<<version<<".cpp\n\nmpirun --mca btl_openib_warn_no_device_params_found 0 "<<version<<" "<< k<<" "<<iterations2<<"\n\nexit 0";
        Slurm.close();
        iterations2 = iterations2/2;
    }//for j
       // }// i
    return 0;
}
