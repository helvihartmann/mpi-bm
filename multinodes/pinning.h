#include <numa.h>
/*19.08.2014 pins processes to specific CPU's*/

class Pinning{
    private:

    public:
    
    Pinning();
    
    void usepinningpattern(int commflag, int pinningmode);

};

Pinning::Pinning(){
    struct bitmask* nodemask = numa_allocate_cpumask();
    numa_bitmask_setbit(nodemask, 0);
    cout << "pinning all processes to numa_node 0" << endl;
    numa_bind(nodemask);
    numa_set_membind(nodemask);
}

void Pinning::usepinningpattern(int commflag, int pinningmode){
    struct bitmask* nodemask = numa_allocate_cpumask();
    switch (pinningmode) {
            
        case 1:{
            cout << "sender and receiver already pinned to numa_node 0" << endl;

        }
            break;
            
        case 2:{
            numa_bitmask_setbit(nodemask, 1);
            cout << "pinning sender and receiver to numa_node 1" << endl;
            numa_bind(nodemask);
            numa_set_membind(nodemask);
        }
            break;
        case 3:{
            if (commflag == 0){
                numa_bitmask_setbit(nodemask, 0);
                cout << "pinning sender to numa_node 0" << endl;
                numa_bind(nodemask);
                numa_set_membind(nodemask);
            }
            else{
                numa_bitmask_setbit(nodemask, 1);
                cout << "pinning receiver to numa_node 1" << endl;
                numa_bind(nodemask);
                numa_set_membind(nodemask);
            }
        }
            break;
        case 4:{
            if (commflag == 0){
                numa_bitmask_setbit(nodemask, 1);
                cout << "pinning sender to numa_node 1" << endl;
                numa_bind(nodemask);
                numa_set_membind(nodemask);
            }
            else{
                numa_bitmask_setbit(nodemask, 0);
                cout << "pinning receiver to numa_node 0" << endl;
                numa_bind(nodemask);
                numa_set_membind(nodemask);
            }
        }
            break;
        default:
            break;
    }
   
    

}