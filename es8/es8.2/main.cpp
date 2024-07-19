#include <iostream>
#include "metropolis.hpp"

using namespace std;



int main (int argc, char *argv[]){
	SA simulation;
	simulation.minimizza();// questo mi fa il file con l'evoluzione di <H> e dei parametri
	// cerr << (simulation.Get_H_accepted()).Get_delta() << endl;
	
	// da qui faccio l'istogramma
	ofstream coutf("./OUTPUT/ground_state.dat");
	coutf << "      # steps:             x:\n";

	metropolis minim_H(true); // messo il true poichè voglio mi faccia il setup di quando misuro tutti i blocchi
	
	// cerr << simulation.Get_H_accepted().Get_u() << endl;

	minim_H.Set_s((simulation.Get_H_accepted()).Get_s());
	minim_H.Set_u((simulation.Get_H_accepted()).Get_u());
	minim_H.Set_delta((simulation.Get_H_accepted()).Get_delta());
	int steps_per_bl=minim_H.Get_nsteps();
	int n_bl=minim_H.Get_nbl();
	int termal=minim_H.Get_termal();; //steps per termalizzare

	for(int i=0; i < termal; i++){
		minim_H.step();
	}

	for(int i=0; i < n_bl; i++){ //loop over blocks
		for(int j=0; j < steps_per_bl; j++){ //loop over steps in a block
			minim_H.step();
			minim_H.measure();
			coutf << setw(12) << i*steps_per_bl+j+1
				<< " " << setw(20) << minim_H.Get_x() << endl;
		}
		minim_H.averages(i+1);
		minim_H.block_reset(i+1);
	}
	minim_H.finalize();

	coutf.close();

	return 0;
}