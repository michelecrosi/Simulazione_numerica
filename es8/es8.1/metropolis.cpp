#include <cmath>
#include <cstdlib>
#include <string>
#include "metropolis.hpp"

metropolis::metropolis(){
	x=0;

	int p1, p2; // Read from ./INPUT/Primes a pair of numbers to be used to initialize the RNG
	ifstream Primes("./INPUT/Primes");
	Primes >> p1 >> p2 ;
	Primes.close();
	int seed[4]; // Read the seed of the RNG
	ifstream Seed("./INPUT/seed.in");
	Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
	_rnd.SetRandom(seed,p1,p2);

	_rnd.SetRandom(seed,p1,p2);

	ofstream couta("./OUTPUT/acceptance.dat"); // Set the heading line in file ./OUTPUT/acceptance.dat
	couta << "    # N_BLOCK:          ACCEPTANCE:" << endl;
	couta.close();

	ifstream input("./INPUT/input.dat"); // Start reading ./INPUT/input.dat
	ofstream coutf;
	coutf.open("./OUTPUT/output.dat");
	string property;
	double delta;
	while ( !input.eof() ){
		input >> property;
		if( property == "DELTA" ){
			input >> delta;
			coutf << "DELTA= " << delta << endl;
			_delta = delta;
		} else if( property == "NBLOCKS" ){
			input >> _nblocks;
			coutf << "NBLOCKS= " << _nblocks << endl;
		} else if( property == "NSTEPS" ){
			input >> _nsteps;
			coutf << "NSTEPS= " << _nsteps << endl;
		} else if( property == "ENDINPUT" ){
			coutf << "Reading input completed!" << endl;
			break;
		} else cerr << "PROBLEM: unknown input" << endl;
	}
	input.close();
	coutf << "System initialized!" << endl;
	coutf.close();


	// proprietà:

	int index_property = 0;
	_nprop = 0;

	_measure_penergy  = false; //Defining which properties will be measured
	_measure_kenergy  = false;
	_measure_tenergy  = false;

	input=ifstream("./INPUT/properties.dat");
	if (input.is_open()){
		while ( !input.eof() ){
			input >> property;
			if( property == "POTENTIAL_ENERGY" ){
			ofstream coutp("./OUTPUT/potential_energy.dat");
			coutp << "    # BLOCK:          ACTUAL_PE:              PE_AVE:              ERROR:" << endl;
			coutp.close();
			_nprop++;
			_index_penergy = index_property;
			_measure_penergy = true;
			index_property++;
			} else if( property == "KINETIC_ENERGY" ){
			ofstream coutk("./OUTPUT/kinetic_energy.dat");
			coutk << "    # BLOCK:          ACTUAL_KE:              KE_AVE:              ERROR:" << endl;
			coutk.close();
			_nprop++;
			_measure_kenergy = true;
			_index_kenergy = index_property;
			} else if( property == "TOTAL_ENERGY" ){
			ofstream coutt("./OUTPUT/total_energy.dat");
			coutt << "    # BLOCK:          ACTUAL_TE:              TE_AVE:              ERROR:" << endl;
			coutt.close();
			_nprop++;
			_measure_tenergy = true;
			_index_tenergy = index_property;
			index_property++;
			} else if( property == "ENDPROPERTIES" ){
			ofstream coutf;
			coutf.open("./OUTPUT/output.dat",ios::app);
			coutf << "Reading properties completed!" << endl;
			coutf.close();
			break;
			} else cerr << "PROBLEM: unknown property" << endl;
		}
		input.close();
	} else cerr << "PROBLEM: Unable to open properties.dat" << endl;

	// according to the number of properties, resize the vectors _measurement,_average,_block_av,_global_av,_global_av2
	_measurement.resize(_nprop);
	_average.resize(_nprop);
	_block_av.resize(_nprop);
	_global_av.resize(_nprop);
	_global_av2.resize(_nprop);
	_average.zeros();
	_global_av.zeros();
	_global_av2.zeros();
	_nattempts = 0;
	_naccepted = 0;

	return;
}

void metropolis :: finalize(){
  _rnd.SaveSeed();
  ofstream coutf;
  coutf.open("./OUTPUT/output.dat",ios::app);
  coutf << "Simulation completed!" << endl;
  coutf.close();
  return;
}

double metropolis::psi(double x_){ // non normalizzata
	return exp(-pow(x_-u,2)/(2*s*s))+exp(-pow(x_+u,2)/(2*s*s));
}

// double metropolis::psi(double x){ // non normalizzata
// 	return exp(-(x-u)/(s*s))+exp(-(x+u)/(s*s))+2.*exp(-(x*x-u*u)/(s*s));
// }


void metropolis::step(){
	double acceptance;
	double xnew=x + _rnd.Rannyu(-_delta, _delta);
	acceptance = pow(psi(xnew)/psi(x),2);
	_nattempts++;
	if(_rnd.Rannyu() < acceptance){
		x=xnew; //Metropolis acceptance step
		_naccepted++;
	}
	return;
}

void metropolis :: block_reset(int blk){ // Reset block accumulators to zero
	ofstream coutf;
	if(blk>0){
		coutf.open("./OUTPUT/output.dat", ios::app);
		coutf << "Block completed: " << blk << endl;
		coutf.close();
	}
	_block_av.zeros();

	return;
}

void metropolis::measure(){
	_measurement.zeros();
	if(_measure_kenergy){
		_measurement(_index_kenergy)=K(x);
	}
	if(_measure_penergy){
		_measurement(_index_penergy)=V(x);
	}
	if (_measure_tenergy){
		_measurement(_index_tenergy)=K(x)+V(x);
		// cout << _measurement(_index_tenergy) << endl;
	}
	_block_av += _measurement; //Update block accumulators
}

double metropolis :: V(double x_){
	// cout << (pow(x_,4)-2.5*pow(x_,2)) << "\t";
	return (pow(x_,4)-2.5*pow(x_,2));
}

double metropolis :: K(double x_){
	// cout << (-0.5*((pow((x_-u)/(pow(s,2)),2)-1./pow(s,2)) *exp(pow(x_-u,2)/(2*pow(s,2))) + (pow((x_+u)/(pow(s,2)),2)-1./pow(s,2)) *exp(pow(x_+u,2)/(2*pow(s,2)))))/psi(x_) << endl;
	return (-0.5*((pow((x_-u)/(pow(s,2)),2)-1./pow(s,2)) *exp(-pow(x_-u,2)/(2.*pow(s,2))) + (pow((x_+u)/(pow(s,2)),2)-1./pow(s,2)) *exp(-pow(x_+u,2)/(2.*pow(s,2)))))/psi(x_)  ;
}
// -0.5 * dx^2(psi)

void metropolis :: averages(int blk){ //questa funzione la chiamo alla fine di un blocco di misure

	ofstream coutf;
	coutf << scientific;
	double average, sum_average, sum_ave2;

	_average = _block_av / double(_nsteps); //media dell'ultimo blocco per ogni proprietà
	_global_av  += _average; //vettore con somma globale per ogni proprietà
	_global_av2 += _average % _average;
	//vettore con somma globale dei quadrati di ogni proprietà dei blocchi

	// POTENTIAL ENERGY //////////////////////////////////////////////////////////

	if(_measure_penergy){
		coutf.open("./OUTPUT/potential_energy.dat",ios::app);
		average  = _average(_index_penergy);
		sum_average = _global_av(_index_penergy);
		sum_ave2 = _global_av2(_index_penergy);
		// coutf << scientific;
		coutf << setw(12) << blk
				<< " " << setw(20) << average //media del blocco blk
				<< " " << setw(20) << sum_average/double(blk) // questa così diviene la media dei primi blk blocchi
				<< " " << setw(20) << this->error(sum_average, sum_ave2, blk) << endl; // questa così diviene l'errore della media dei primi blk blocchi
		coutf.close();
	}
	// KINETIC ENERGY ////////////////////////////////////////////////////////////

	if(_measure_kenergy){
		coutf.open("./OUTPUT/kinetic_energy.dat",ios::app);
		average  = _average(_index_kenergy);
		sum_average = _global_av(_index_kenergy);
		sum_ave2 = _global_av2(_index_kenergy);
		coutf << setw(12) << blk
				<< " " << setw(20) << average
				<< " " << setw(20) << sum_average/double(blk)
				<< " " << setw(20) << this->error(sum_average, sum_ave2, blk) << endl; //<< scientific;
		coutf.close();
	}

	// TOTAL ENERGY //////////////////////////////////////////////////////////////
	if (_measure_tenergy){
		coutf.open("./OUTPUT/total_energy.dat",ios::app);
		average  = _average(_index_tenergy);
		sum_average = _global_av(_index_tenergy);
		sum_ave2 = _global_av2(_index_tenergy);
		coutf << setw(12) << blk
			<< " " << setw(20) << average
			<< " " << setw(20) << sum_average/double(blk)
			<< " " << setw(20) << this->error(sum_average, sum_ave2, blk) << endl; //<< scientific;
		coutf.close();
	}

	// ACCEPTANCE ////////////////////////////////////////////////////////////////
	double fraction;
	coutf.open("./OUTPUT/acceptance.dat",ios::app);
	if(_nattempts > 0) fraction = double(_naccepted)/double(_nattempts);
	else fraction = 0.0;
	coutf << setw(12) << blk << " " << setw(20) << fraction << endl;
	coutf.close();

	return;
}

double metropolis :: error(double acc, double acc2, int blk){
  if(blk <= 1) return 0.0;
  else return sqrt( fabs(acc2/double(blk) - pow( acc/double(blk) ,2) )/double(blk) );
}

int metropolis :: get_nbl(){
  return _nblocks;
}

int metropolis :: get_nsteps(){
  return _nsteps;
}

void metropolis::Setu(double mu){
	u=mu;
}

void metropolis::Sets(double sigma){
	s=sigma;
}