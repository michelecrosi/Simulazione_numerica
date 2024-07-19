


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
	private:
		double s;
		double u;
		double x;

		Random _rnd;
		int _nblocks;         // Number of blocks for block averaging
		int _nsteps;          // Number of simulation steps in each block
		double _delta; 

		int _nprop; // Number of properties being measured
  		bool _measure_penergy, _measure_kenergy, _measure_tenergy;
		int _index_penergy, _index_kenergy, _index_tenergy;       // Indices for accessing energy-related properties in vec _measurement
		vec _block_av;         // Block averages of properties
		vec _global_av;        // Global averages of properties
		vec _global_av2;       // Squared global averages of properties
		vec _average;          // Average values of properties
		vec _measurement; 
	public: // Function declarations
		metropolis(); 
		void Setu(double mu);
		void Sets(double sigma);
		
		double psi(double x_);
		double V(double x_);
		double K(double x_);

		int _nattempts;       // Number of attempted moves
		int _naccepted;

		int get_nbl();              // Get the number of blocks
		int get_nsteps();           // Get the number of steps in each block
		void initialize_properties();// Initialize properties for measurement
		void finalize();            // Finalize system and clean up
		void step();                // Perform a simulation step
		void block_reset(int blk);  // Reset block averages
		void measure();             // Measure properties of the system
		void averages(int blk);     // Compute averages of properties
		double error(double acc, double acc2, int blk);
};



#endif