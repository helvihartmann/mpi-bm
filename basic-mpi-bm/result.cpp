#include <iostream>
#include <vector>

using namespace std;

void output(int rank, unsigned int nmbr_commprocess, vector<double> time, vector<size_t> packagesize, vector<size_t>innerRuntimeIterations){
    if(rank == 0){
        for (int phase = 0; phase < nmbr_commprocess/2; phase++){
            for (unsigned int i = 0; i < packagesize.size(); i++){
                int index = i+(phase*packagesize.size());
                cout << packagesize.at(i)*innerRuntimeIterations.at(i) << " " << innerRuntimeIterations.at(i) << " "<< packagesize.at(i) << " " << time.at(index) << " " << (packagesize.at(i)*innerRuntimeIterations.at(i))/(time.at(index)*1000000) << endl;
            }
           cout << " " << endl;
        }
    }
}
