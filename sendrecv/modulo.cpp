
#include <iostream>

/* fles:~/benchmark/sendrecv 30.01.14
 this is the main.cpp (pingpong.cpp) to perform a MPI benchmark where process 0 sends data to process 1 and gets it back, for which the time is meassured in order to calculate the data rate*/

int main(int argc,char *argv[]){
    unsigned int size = 8; 
    for (unsigned int i = 0; i<size; i++){
        std::cout << "previous: " << (i-1)%size << "  "<< i << "  next: " << (i+1)%size << std::endl;
    }
    return 0;
    
}
