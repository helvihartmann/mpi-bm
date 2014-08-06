#include <numa.h>
#include <iostream>

/* 06.08.2014 gcc numa.cpp -o numa -lstdc++ -std=c++11 -lnuma */
int main(){
    
    //int avlbl = numa_available();
    int cpu = 1;
    if(-1 == numa_node_of_cpu(cpu)){
        std::cout << "you are on cpu 0" << std::endl;
    }
    else{
        std::cout <<"you are on cpu 1" << std::endl;
    }
    
    struct bitmask* nodemask = numa_allocate_cpumask();
    numa_bitmask_setbit(nodemask, 0);
    std::cout << "pinning to node 0" << std::endl;
    numa_bind(nodemask);

    if(-1 == numa_node_of_cpu(cpu)){
        std::cout << " error you are on cpu 0" << std::endl;
    }
    
    return 0;
}