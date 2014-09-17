//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include "string.h"
#include "tsc.h"

using namespace std;

/*gcc server.cpp -L/opt/fairsoft/jul14p3_build/lib -lzmq -I/opt/fairsoft/jul14p3_build/include -std=c++11 -o server -Wl,-rpath=/opt/fairsoft/jul14p3_build/lib/
 last options forces executable to search libraries in the specified directory
 */

int main () {
    //  Prepare our context and socket
    zmq::context_t context (1); //io_threads=1, specifies size of the thread pool
    zmq::socket_t socket (context, ZMQ_REP);//initiates a reply socket
    socket.bind ("tcp://*:5555");//endpoint string: transport://adress
    
    while (true) {
        zmq::message_t request;//object of message_t
        
        //  Wait for next request from client
        socket.recv (&request);
        //std::string rpl = std::string(static_cast<char*>(request.data()), request.size());
        //std::cout << "Received " << request.size() << "B" << std::endl;
        
        //  Send reply back to client
        String message(request.size());
        string str = *message.createstring();
        zmq::message_t reply (request.size());
        memcpy ((void *) reply.data (), str.c_str(), request.size());
        socket.send (reply);
    }
    return 0;
}