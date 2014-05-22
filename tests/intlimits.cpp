#include <limits>
#include <iostream>
#include <cstddef>
using namespace std;

int main() {
	size_t sizemax = std::numeric_limits<size_t>::max();
	int imin = std::numeric_limits<int>::min(); // minimum value

	int imax = std::numeric_limits<int>::max();
	cout<<"min: "<<imin<<"\n";
	cout<<"max: "<<imax<<"\n";
	cout<<"max size_t: " << sizemax << endl;
}

