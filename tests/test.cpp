#include <iostream>

using namespace std;


int main(){
    
    int size = 64;
    int iterations = 5;
	int Zahlen[size];
    
	//int *ZahlZeiger;
	
	for (int z=0; z<size; z++){
		Zahlen[z]=z;
	}

	/*ZahlZeiger = Zahlen;
	ZahlZeiger = &Zahlen[2];
	cout << *ZahlZeiger << endl;
	ZahlZeiger = &Zahlen[5];
	cout << *ZahlZeiger << endl;*/
    
    cout << Zahlen[5] << endl;
    cout << Zahlen[1] << endl;
    
    for (int p=2; p<size; p=p*2){//puffer size
        int idx = 0;
        for (int i=0; i<iterations; i++){
            int idx_c = (idx*p)%size;
            cout << p << " " << idx_c << " " << Zahlen[idx_c] << endl;
            idx++;
        }
        
	}


return 0;
}
