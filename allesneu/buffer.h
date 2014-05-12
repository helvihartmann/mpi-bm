#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
/**/

class Buffer{
private:
    int size;
    int rank;
    
    int pipelinedepth;
    int numberofRootProcesses;
    size_t buffersize;

    
public:
    
    Buffer(int size, int rank, int pipelinedepth, int numberofrootprocesses, size_t buffersize);
    
};

#endif /*BUFFER_H*/