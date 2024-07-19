


#ifndef __metropolis__
#define __metropolis__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <armadillo>
#include <stdlib.h> //exit
#include "random.h"

using namespace std;
using namespace arma;

class metropolis{
	protected:
		double s;
		double u;
		double x;

		Random _rnd;
		int _nblocks;         // Number of blocks for block averaging
		int _nsteps;          // Number of simulation steps in each block
		int _nattempts;       // Number of attempted moves
		int _naccepted;
		double _delta; 

		int termal; //steps per termalizzare
		
		int _nprop; // Number of properties being measured
  		bool _measure_penergy, _measure_kenergy, _measure_tenergy;
		int _index_penergy, _index_kenergy, _index_tenergy;       // Indices for accessing energy-related properties in vec _measurement
		vec _block_av;         // Block averages of properties
		vec _global_av;        // Global averages of properties
		vec _global_av2;       // Squared global averages of properties
		vec _average;          // Average values of properties
		vec _measurement; 
		long double Pacc(double x_old, double x_new);

	public: // Function declarations
		// metropolis();
		metropolis(bool b);
		double Get_u();
		double Get_s(); 
		void Set_u(double mu);
		void Set_s(double sigma);
		void Set_delta(double D);
		
		long double psi(double x_);
		long double V(double x_);
		long double K(double x_);

		double Get_x();
		double Get_delta();
		int Get_nbl();              // Get the number of blocks
		int Get_nsteps();           // Get the number of steps in each block
		int Get_termal();			// Get the number of steps to equilibrate the metropolis
		void initialize_properties();// Initialize properties for measurement
		void finalize();            // Finalize system and clean up
		void step();                // Perform a simulation step
		void block_reset(int blk);  // Reset block averages
		virtual void measure();             // Measure properties of the system
		virtual void averages(int blk);     // Compute averages of properties
		double error(double acc, double acc2, int blk);
};

class H: public metropolis{ // simile a: "metropolis" ma ti calcola direttamente l'energia media ed errore associato senza stamparti tutti i valori di ogni blocco
	protected:
		void block_reset();
		void measure() override;
		void averages(int blk) override;
		double err_H;
		double accettazione;
		double _H;// valore medio di H in uno stato psi
	public:
		H();
		void H_fin();
		double Get_H();
		double Get_err_H();
		double Get_accettazione();
		// void operator=(const H& h2);

};

class SA{
	protected:
		double delta;
		double nattempts;
		double naccepted;
		Random _rnd;
		int iterations; // iterazioni in uno step SA(numero configurazioni indagate per ogni temperatura)
		double T;
		double T_i;
		double T_f;
		int n_Temperature;
		H H_accepted;
		H H_try;
		// H _H;
	public:
		SA();
		H Get_H_accepted();
		double T_function(double t);
		void SA_metro();
		void minimizza();		

};

#endif