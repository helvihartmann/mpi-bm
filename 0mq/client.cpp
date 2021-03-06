//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include "string.h"

using namespace std;
using namespace std::chrono;
//gcc client.cpp -L/opt/fairsoft/jul14p3_build/lib -lzmq -I/opt/fairsoft/jul14p3_build/include -std=c++11 -o client -Wl,-rpath=/opt/fairsoft/jul14p3_build/lib/

int main (int argc, char **argv)
{
    if (argc != 2) {
        cerr << "usage: client <target>" << endl;
        return 1;
    }
    
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
    
    cout << "Connecting to server at " << argv[1] << endl;
    socket.connect (argv[1]);
    
    size_t inneriter = 100000;
    zmq::message_t reply;
    
    cout << "datatot[B] iter pckgsize[B] time[s] error rate[MB/s] error rate[Mb/s]" << endl;
    for(size_t messagesize = 2; messagesize <= 16777216; messagesize = messagesize *2){
        inneriter = inneriter/2;
        if(inneriter < 200){
            inneriter = 200;
        }
        String message(messagesize);
        string str = *message.createstring();
        //std::cout << "Sending " << inneriter << " times " << messagesize << "B" << std::endl;
        
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int request_nbr = 0; request_nbr != inneriter; request_nbr++) {
            zmq::message_t request (messagesize);
            memcpy ((void *) request.data (), str.c_str(), messagesize);
            socket.send (request);
            socket.recv (&reply);
        }
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        //std::string rpl = std::string(static_cast<char*>(reply.data()), reply.size());
        double time = time_span.count()/2;
        cout << inneriter*messagesize << "      " << inneriter << "  " << reply.size() << "        " << time << " -    " << ((inneriter*messagesize)/time)/1000000 << " -     " << (((inneriter*messagesize)/time)/1000000)*8 << endl;
    }
    
    return 0;
}