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
    
    void setPackagesizeTmp(size_t p);
    
    size_t getpackagesize(int &argc, char **argv);
        
    size_t getcutoff(int &argc, char **argv);

/* calculate the number of sending the package to have process occur in seconds regime
 sending 128 5millions time took a reasonable time, thats where the empiricalfactor comes from*/
    size_t getiterations2();
    
    size_t getTotalDataSent();
    
};