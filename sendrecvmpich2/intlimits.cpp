#include <limits>
#include <iostream>
using namespace std;

int main() {
	int imin = std::numeric_limits<int>::min(); // minimum value
	int imax = std::numeric_limits<int>::max();
	cout<<"min: "<<imin<<"\n";
	cout<<"max: "<<imax<<"\n";
}

