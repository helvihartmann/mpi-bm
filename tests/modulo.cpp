
#include <iostream>


int main(int argc,char *argv[]){
    unsigned int size = 8; 
    for (unsigned int i = 0; i<size; i++){
        std::cout << "previous: " << (i-1)%size << "  "<< i << "  next: " << (i+1)%size << std::endl;
    }
    return 0;
    
}
