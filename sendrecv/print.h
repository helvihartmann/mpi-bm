#include <fstream>
#include <string>
#include <iostream>
#include <cstddef>

/* pfad
27.11.2013
prints everything into output*/

class Printoutput{
public:
    void printtimestemp();
    
    void printheader();
    
    void printerrormessage(size_t*,size_t);
    
    void printbandwidth(size_t, size_t, double , long double , double , long double , double , double*);
    
};
