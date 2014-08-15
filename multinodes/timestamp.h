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

void Timestamp::printtimestamp(){
    Zeitstempel = time(0);
    nun = localtime(&Zeitstempel);
    cout << "#" << nun->tm_mday << '.' << nun->tm_mon+1 << '.'<< nun->tm_year+1900 << " - " << nun->tm_hour << ':' << nun->tm_min << ':' << nun->tm_sec << endl;
}