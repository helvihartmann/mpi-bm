#include "buffer.h"

Buffer::Buffer(int size_, int rank_, int pipelinedepth_, int numberofRootProcesses_, size_t buffersize_) :
    size(size_),
    rank(rank_),
    pipelinedepth(pipelinedepth_),
    numberofRootProcesses(numberofRootProcesses_),
    buffersize(buffersize_)
{
    std::cout << " class buffer reporting for duty" << std::endl;
}