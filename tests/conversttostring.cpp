#include <iostream>
#include <sstream>

using namespace std;

int main(int argc,char *argv[]){
    int i = 10;
    
    ostringstream oss;
    oss << "text" << i;
    cout << oss.str();
    cout << "hase" << endl;
    
    
}
