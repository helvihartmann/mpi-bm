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
    int i,s;
    double k,j;
    string stichwort, h;
    
    // this loop is for creating the amount of data referred to as k000000Byte
    for(j=1; j<6;j++){
        
            // for number of processes that exchange data
            for(i=2; i<=8; i++){
                stringstream streami;
                streami <<i;
                
                double basis = 2;
                k=pow(basis,j);
                stringstream streamk;
                streamk <<k;
                string name="slurm_";
                name+=streamk.str().c_str();
                name+="MB.cpp";
                fstream Slurm;
                name = streami.str()+'/'+name;
                
                Slurm.open(name.c_str(), ios::in|ios::binary);
                //Auslesen
                bool found = false;
                while (!h.eof() && !found)      // Bis nix mehr zum lesen gibt ... ausführen
                {
                    
                    getline(h, s);    // Das liest jede zeile ...
                    stichwort = streami.str().c_str();
                    stichwort+ = " took";
                    if(s == stichwort){
                        found = true;
                    }
                    if(abbrechen) cout << s << endl;// Das gelesene wird ausgegeben
                }

                Slurm.close();
            }//for i
        }// for k
    return 0;
}


