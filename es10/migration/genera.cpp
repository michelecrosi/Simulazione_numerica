#include <iostream>
#include <string>
#include <fstream>
#include <iomanip> //setw()
#include "random.h"

using namespace std;



int main (int argc, char *argv[]){
	bool square=false;
	bool circ=false;
	Random rnd;
	
	// for(int i=0;i<20;i++) {
	// 	cerr << "\n" << endl;
	// 	cerr << rnd.Rannyu() << endl;
	// 	cerr << rnd.Rannyu(-0.5,0.5) << endl;	
	// }
	
	int n_cities;
	string disposition;
	ifstream input("./INPUT/input_genera.dat");

	string property;
	while ( !input.eof() ){
		input >> property;
		if( property == "N_CITIES" ){
			input >> n_cities;
			if(n_cities <= 0){
				cerr << "PROBLEM: number of cities must be positive" << endl;
			return -999;
			}
		} else if( property == "SHAPE" ){
			input >> disposition;
				if(disposition=="SQUARE") square=true;
				if(disposition=="CIRCUMFERENCE") circ=true;
				if(disposition=="SQUARE_AND_CIRCUMFERENCE"){
					circ=true;
					square=true;
				}
		} else if( property == "ENDINPUT" ){
			break;
		} else{
			cerr << "PROBLEM: unknown disposition of cities" << endl;
			return -9999;
		} 
	}
	input.close();

	ofstream coutf;
	// uso come untità di misura il lato del quadrato
	if(square){
		coutf.open("./OUTPUT/square.dat");
		coutf << scientific;
        coutf << "    NUMERO_CITTA':          x:                 y:" << endl;
		for(int i=0; i<n_cities; i++){
			coutf << setw(12) << i+1 << setw(20) << rnd.Rannyu(-0.5,0.5) 
          		<< " " << setw(20) << rnd.Rannyu(-0.5,0.5) << endl; 
		}
		coutf.close();
	}
	

	// uso come untità di misura il raggio della circonferenza
	if(circ){
		coutf.open("./OUTPUT/circumference.dat");
		cord c(0,0); // messo un valore iniziale che non può essere generato in modo da capire se ho un malfunzionamento
        coutf << "    NUMERO_CITTA':          x:                 y:" << endl;
		for(int i=0; i<n_cities; i++){
			c=rnd.vers_2D_using_pi();
			coutf << setw(12) << i+1 << setw(20) << c.x 
          		<< " " << setw(20) << c.y << endl; 
		}
	}
	
	return 0;
}