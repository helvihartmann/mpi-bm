#ifndef TOTAL_DATA
#define TOTAL_DATA

#include <cstddef>
#include <cstdlib>
/* pfad
 14.11.2013
*/

    //get starting packege size; read in data to send from console (default =128B)
class Totaldatasendcalc{
private:
    int sendmode = 1; //send by default
    size_t startPackageSize = 2;
    size_t cutoff = 1000;
    size_t iterations2;
    size_t empiricalfactor = 5000000*128;
    size_t packagesize_temp;

public:
    
    void setPackagesizeTmp(size_t);
    
    void readOptions(int&, char**);
    
    size_t getpackagesize();
        
    size_t getcutoff();
    
    int const getsendmode();

/* calculate the number of sending the package to have process occur in seconds regime
 sending 128 5millions time took a reasonable time, thats where the empiricalfactor comes from*/
    size_t getiterations2();
    
    size_t getTotalDataSent();
    
};
#endif