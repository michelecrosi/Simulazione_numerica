#include <iostream>
#include "metropolis.hpp"

using namespace std;



int main (int argc, char *argv[]){
	metropolis SYS;
	SYS.Setu(0.05);
	SYS.Sets(0.1);
	SYS.block_reset(0);

	int termal=100000; //steps per termalizzare
	for(int i=0; i < termal; i++){
		SYS.step();
	}

	// cout << SYS._naccepted << "\t" << SYS._nattempts << endl;


	for(int i=0; i < SYS.get_nbl(); i++){ //loop over blocks
		for(int j=0; j < SYS.get_nsteps(); j++){ //loop over steps in a block
			SYS.step();
			SYS.measure();
		}
		SYS.averages(i+1);
		SYS.block_reset(i+1);
	}
	SYS.finalize();

	return 0;
}