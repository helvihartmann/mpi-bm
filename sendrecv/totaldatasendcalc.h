//#include <stdlib.h>

/* pfad
 14.11.2013
*/

    //get starting packege size; read in data to send from console (default =128B)
class Totaldatasendcalc{
private:
    size_t ncounts;
    int cutoff;
    size_t iterations2;
    size_t empiricalfactor = 5000000*128;
    size_t packagesize_temp;
public:
    
    void setPackagesizeTmp(size_t p){
        packagesize_temp = p;
    }
    
    size_t getpackagesize(int &argc, char **argv){
        size_t ncounts;
        if (argc > 0){
            ncounts = atoll(argv[1]);
        }
        else{
            ncounts = 128;
        }
        return ncounts;
    }
        
    size_t getcutoff(int &argc, char **argv){
        size_t ncounts;
        if (argc > 1){
            cutoff = atoll(argv[2]);
        }
        else{
            cutoff = 8000000;
        }
        return cutoff;
    }

/* calculate the number of sending the package to have process occur in seconds regime
 sending 128 5millions time took a reasonable time, thats where the numbers in the calculation come from*/
    size_t getiterations2(){
        iterations2 = empiricalfactor/packagesize_temp;
        return iterations2;
    }
    
    size_t getTotalDataSent(){
        size_t totalDataSent = iterations2*packagesize_temp*sizeof(int);
        //std::cout<<"totalData: "<< totalDataSent <<" "<< iterations2<<" "<<packagesize_temp<<"\n";
        return totalDataSent;
    }
    
};