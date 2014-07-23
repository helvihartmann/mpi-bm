#include <boost/thread.hpp>

using namespace std;

void task1() {
    cout << "thread 1" << endl;
}

void task2() {
    cout << "thred 2" << endl;
}

int main (int argc, char ** argv) {
    using namespace boost;
    
    thread thread_1 = thread(task1);
    thread thread_2 = thread(task2);
    
    // do other stuff
    thread_2.join();
    thread_1.join();
    return 0;
}