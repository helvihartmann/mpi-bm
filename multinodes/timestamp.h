#include <iostream>

using namespace std;
/*17.07.2014 extra for Dominic Eschweiler*/

class Timestamp{
    private:
        time_t Zeitstempel;
        tm *nun;
    public:
    
    void printtimestamp();
};

