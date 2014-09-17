//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>


/*gcc helloworld_server.cpp -L/opt/fairsoft/jul14p3_build/lib -lzmq -I/opt/fairsoft/jul14p3_build/include -std=c++11 -o helloworld_server -Wl,-rpath=/opt/fairsoft/jul14p3_build/lib/
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
        std::cout << "Received Hello" << std::endl;
        
        //  Do some 'work'
        sleep(1);
        
        //  Send reply back to client
        zmq::message_t reply (5);
        memcpy ((void *) reply.data (), "World", 5);
        socket.send (reply);
    }
    return 0;
}