#include <cmath>
#include <cstdlib>
#include <string>
#include "metropolis.hpp"

// bool bole=false;
// int con2=0;
// int contatore=0;

metropolis::metropolis(bool stampa){
	x=0;
	// cout << stampa << endl;

	int p1, p2; // Read from ./INPUT/Primes a pair of numbers to be used to initialize the RNG
	ifstream Primes("./INPUT/Primes");
	Primes >> p1 >> p2 ;
	Primes.close();
	int seed[4]; // Read the seed of the RNG
	ifstream Seed("./INPUT/seed.in");
	Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
	_rnd.SetRandom(seed,p1,p2);

	if(stampa){
		ofstream couta("./OUTPUT/acceptance.dat"); // Set the heading line in file ./OUTPUT/acceptance.dat
		couta << "    # N_BLOCK:          ACCEPTANCE:" << endl;
		couta.close();
	}

	ifstream input("./INPUT/input.dat"); // Start reading ./INPUT/input.dat
	
	ofstream coutf;
	
	if(stampa){
		coutf.open("./OUTPUT/output.dat");
	}
	
	string property;
	double delta;
	while ( !input.eof() ){
		input >> property;
		if( property == "DELTA" ){
			input >> delta;
			if(stampa) coutf << "DELTA= " << delta << endl;
			_delta = delta;
			// cout << _delta;
		} else if( property == "NBLOCKS" ){
			input >> _nblocks;
			if(stampa) coutf << "NBLOCKS= " << _nblocks << endl;
		} else if( property == "NSTEPS" ){
			input >> _nsteps;
			if(stampa) coutf << "NSTEPS= " << _nsteps << endl;
		} else if( property == "TERMALSTEPS" ){
			input >> termal; 
			// cerr << termal << endl;
			if(stampa) coutf << "TERMALSTEPS= " << termal << endl;
		} else if( property == "ENDINPUT" ){
			if(stampa) coutf << "Reading input completed!" << endl;
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
				if(stampa){	
					ofstream coutp("./OUTPUT/potential_energy.dat");
					coutp << "    # BLOCK:          ACTUAL_PE:              PE_AVE:              ERROR:" << endl;
					coutp.close();
				}
				_nprop++;
				_index_penergy = index_property;
				_measure_penergy = true;
				index_property++;
			} else if( property == "KINETIC_ENERGY" ){
				if(stampa){	
					ofstream coutk("./OUTPUT/kinetic_energy.dat");
					coutk << "    # BLOCK:          ACTUAL_KE:              KE_AVE:              ERROR:" << endl;
					coutk.close();	
				}	
				_nprop++;
				_measure_kenergy = true;
				_index_kenergy = index_property;
			} else if( property == "TOTAL_ENERGY" ){
				if(stampa){	
					ofstream coutt("./OUTPUT/total_energy.dat");
				coutt << "    # BLOCK:          ACTUAL_TE:              TE_AVE:              ERROR:" << endl;
				coutt.close();
				}
			_nprop++;
			_measure_tenergy = true;
			_index_tenergy = index_property;
			index_property++;
			} else if( property == "ENDPROPERTIES" ){
				if(stampa){
					ofstream coutf;
					coutf.open("./OUTPUT/output.dat",ios::app);
					coutf << "Reading properties completed!" << endl;
					coutf.close();
				}
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
	_block_av.zeros();
	_average.zeros();
	_global_av.zeros();
	_global_av2.zeros();
	_nattempts = 0;
	_naccepted = 0;

	// cout << index_property << endl;

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

long double metropolis::psi(double x_){ // non normalizzata
	// cout << u << "\t" << s << "\t" << exp(-pow(-u,2)/(2.*s*s)) << "\t" << exp(-pow(u,2)/(2.*s*s)) << endl;
	return (exp(-pow(x_-u,2)/(2.*s*s))+exp(-pow(x_+u,2)/(2.*s*s)));
}

// probabilità di accettare la mossa (può essere più di 1 in quel caso posso pensarla a 1)
long double metropolis::Pacc(double x_old, double x_new){ 
// if(psi(x_old)==0){
// 	cerr << x_old << "\t" << x_new << "\t" << u << "\t" << s << "\n"; 
// 	cerr << pow(((1.+exp(-(2.*x_new*u)/(pow(s,2)))) / (1.+ exp(-(2.*x_old*u)/(pow(s,2))))),2) * exp((x_old*(x_old-2.*u)-x_new*(x_new-2.*u))/(pow(s,2))) << "\t" << pow(psi(x_new)/psi(x_old),2) << endl;
// }
// if(_rnd.Rannyu(-_delta, _delta)<0.0001){
// 	cerr << "rand" << pow(((1.+exp(-(2.*x_new*u)/(pow(s,2)))) / (1.+ exp(-(2.*x_old*u)/(pow(s,2))))),2) * exp((x_old*(x_old-2.*u)-x_new*(x_new-2.*u))/(pow(s,2))) << "\t";
// 	cerr << pow(psi(x_new)/psi(x_old),2) << endl;
// }
	return (pow(((1.+exp(-(2.*x_new*u)/(pow(s,2)))) / (1.+ exp(-(2.*x_old*u)/(pow(s,2))))),2) * exp((x_old*(x_old-2.*u)-x_new*(x_new-2.*u))/(pow(s,2))));
}

// double metropolis::psi(double x){ // non normalizzata
// 	return exp(-(x-u)/(s*s))+exp(-(x+u)/(s*s))+2.*exp(-(x*x-u*u)/(s*s));
// }


void metropolis::step(){

// con2++;
// bole=(((con2/termal)>=(577-1) and (con2/termal)<=577+7) and (con2%termal)==0);
// bole=(((con2/termal)>=(723-20) and (con2/termal)<=723+7));


	double acceptance;
	double Dx=_rnd.Rannyu(-_delta, _delta);
	double xnew=x + Dx;
// if(contatore==18)cerr << _delta << "\t";
// if(contatore==18)cerr << _rnd.Rannyu(-_delta, _delta) << "\t";
// if(bole){
// 	cerr << _delta << "\t" << Dx << "\t" << x << "\t" << xnew<< "\t" << psi(x)<< "\t" << psi(xnew)<< "\t" << s << "\t" << u << "\t" << K(x) << "\n";
// 	// cerr << endl << con2 << endl;
// } 

	// cout << _delta << "\n";
	// if(xnew==x) acceptance = 1.; // necessario questo controllo altrimenti la formula sotto mi da problemi se sigma è molto basso e anche x (psi vale 0)
	// else acceptance = pow(psi(xnew)/psi(x),2);

	acceptance = pow(psi(xnew)/psi(x),2);

	// acceptance=Pacc(x,xnew);


// if(contatore==18) cerr << psi(0) << "\t" << s << "\n";

// if(bole){
// 	if(contatore==18){
// 	cerr << "\n\nfjaòfkja\n";
// 	}
// } 

	// cout << xnew << "\t" << x << "\t" << acceptance << endl;
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
	}
	_block_av += _measurement; //Update block accumulators
}

long double metropolis :: V(double x_){
	return pow(x_,4)-2.5*pow(x_,2);
}

long double metropolis :: K(double x_){
// double A=-0.5*(((pow(x_,2)+pow(u,2)-pow(s,2))/(pow(s,4)))+((2.*x_*u)/(pow(s,4)))*(1.-(2./(1.+exp(-(2.*x_*u)/(pow(s,2)))))));
// double B=(-0.5*((pow((x_-u)/(pow(s,2)),2)-1./pow(s,2)) *exp(-pow(x_-u,2)/(2.*pow(s,2))) + (pow((x_+u)/(pow(s,2)),2)-1./pow(s,2)) *exp(-pow(x_+u,2)/(2.*pow(s,2)))))/psi(x_);
// if( fabs(A-B)>0.02  ){
// 	cerr <<  x_ << "\t" << u << "\t" << s << "\n"; 
// 	cerr << A << "\t" << B << "\t" << fabs(A-B) << "\t" << V(x_) << endl;
// }
// if(_rnd.Rannyu(-_delta, _delta)<0.0001){
// 	cerr <<  x << "\t" << u << "\t" << s << "\n"; 
// 	cerr << "rand" << -0.5*(((pow(x,2)+pow(u,2)-pow(s,2))/(pow(s,4)))+((2.*x*u)/(pow(s,4)))*(1.-(2./(1.+exp(-(2.*x*u)/(pow(s,2)))))))
// 		<< "\t" << (-0.5*((pow((x_-u)/(pow(s,2)),2)-1./pow(s,2)) *exp(-pow(x_-u,2)/(2.*pow(s,2))) + (pow((x_+u)/(pow(s,2)),2)-1./pow(s,2)) *exp(-pow(x_+u,2)/(2.*pow(s,2)))))/psi(x_) << endl;
// }
	// return(-0.5*(((pow(x_,2)+pow(u,2)-pow(s,2))/(pow(s,4)))+((2.*x_*u)/(pow(s,4)))*(1.-(2./(1.+exp(-(2.*x_*u)/(pow(s,2))))))));

	// cout << (-0.5*((pow((x_-u)/(pow(s,2)),2)-1./pow(s,2)) *exp(pow(x_-u,2)/(2*pow(s,2))) + (pow((x_+u)/(pow(s,2)),2)-1./pow(s,2)) *exp(pow(x_+u,2)/(2*pow(s,2)))))/psi(x_) << endl;
	
	return (-0.5*((pow((x_-u)/(pow(s,2)),2)-1./pow(s,2)) *exp(-pow(x_-u,2)/(2.*pow(s,2))) + (pow((x_+u)/(pow(s,2)),2)-1./pow(s,2)) *exp(-pow(x_+u,2)/(2.*pow(s,2)))))/psi(x_);
	// non posso usare questa valutazione perchè altrimenti ho troncamenti elevati se s è piccola
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

double metropolis::Get_u(){
	return u;
}

double metropolis::Get_s(){
	return s;
}

void metropolis::Set_u(double mu){
	u=mu;
}

void metropolis::Set_s(double sigma){
	s=sigma;
}

void metropolis::Set_delta(double D){
	_delta=D;
}

double metropolis::Get_x(){
	return x;
}	

double metropolis::Get_delta(){
	return _delta;
}

int metropolis :: Get_nbl(){
	return _nblocks;
}

int metropolis :: Get_nsteps(){
	return _nsteps;
}

int metropolis :: Get_termal(){
	return termal;
}


H::H() : metropolis(false){
	_H=0.;
	err_H=0.;
	accettazione=0.;
	return;
}

double H::Get_H(){
	return _H;
}

double H::Get_err_H(){
	return err_H;
}

double H::Get_accettazione(){
	return accettazione;
}

void H :: block_reset(){
	_block_av.zeros();

	return;
}

void H::measure(){
	_measurement.zeros();
	_measurement(_index_tenergy)=K(x)+V(x);
	_block_av += _measurement; //Update block accumulators
}

void H :: averages(int blk){ //questa funzione la chiamo alla fine di un blocco di misure

	ofstream coutf;
	coutf << scientific;
	double average, sum_average, sum_ave2;

	_average = _block_av / double(_nsteps); //media dell'ultimo blocco per ogni proprietà
	_global_av  += _average; //vettore con somma globale per ogni proprietà
	_global_av2 += _average % _average;

	// TOTAL ENERGY //////////////////////////////////////////////////////////////
	average  = _average(_index_tenergy);
	sum_average = _global_av(_index_tenergy);
	sum_ave2 = _global_av2(_index_tenergy);
	if (blk==_nblocks){ // voglio sapere solo le informazioni sull'ultimo blocco
		_H=sum_average/double(blk);
		err_H=error(sum_average, sum_ave2, blk);
	}
	
	if(_nattempts > 0) accettazione = double(_naccepted)/double(_nattempts);
	else accettazione = 0.0;
	
	return;
}

void H::H_fin(){
	int c=0;
// cerr << setprecision(20);
	do{// cerco un delta che mi faccia una accettazione del 50% basandomi sull'equilibrazione
		x=0.;
		// _naccepted=0;
		// _nattempts=0;
	// cerr <<  accettazione << endl;
	// cerr <<  (accettazione < 0.1) << endl;
		for(int i=0; i < termal; i++){
			step();
	// con2++;
		}
		_naccepted=0;
		_nattempts=0;

		for(int i=0; i < 10 * _nsteps; i++){
			step();
		}


	// if(_nattempts<0) cerr << _nattempts << "\n\n\n\n\n\n\n\n";
		
		if(_nattempts > 0) accettazione = double(_naccepted)/double(_nattempts);
		else accettazione = 0.0;

	// cerr <<  accettazione << endl;
	// bole=(c>16);
	// bole =(accettazione ==0);

	
		
		// _delta*=(accettazione/(1.-accettazione));
	// if(bole){
	// 	contatore ++;
	// 	cerr << "\n\n\n" << con2 << endl;
	// 	// cerr << contatore<< endl;

	// 	cerr << "fjakfaò" << "\t" << c << "\t" << accettazione << "\t" << _delta << "\t" <<(accettazione*2.) << "\t" << u <<"\t" <<s << endl;
	// }else{
	// 	// contatore=0;
	// 	// cerr << "reiari\n\n";
	// }
		if (accettazione==1.){
			_delta=100*(_delta+0.01); // il termine sommato serve nel caso io abbia _delta praticamente pari a 0
		// cerr << _delta << endl; 
		} 
		else if(fabs(accettazione-0.5)>0.02 && accettazione>0.5) _delta*=(0.5/(1.-accettazione));
		else if(fabs(accettazione-0.5)>0.02 && accettazione<=0.5) _delta*=(accettazione*2.);
		c++;

	// if(bole){
	// 	cerr << c << "\t" << accettazione << "\t" << _delta << endl<< endl;
	// } 

	}while(fabs(accettazione-0.5)>0.02 && c<20);

// for(int i=0; i < termal; i++){
// 	step();
// // con2++;
// }


	for(int i=0; i < Get_nbl(); i++){ //ciclo sui blocchi
		for(int j=0; j < Get_nsteps(); j++){ //ciclo sugli step in un blocco
			step();
			measure();
	// bole=(((con2/termal)>=(106755-16) and (con2/termal)<=106755) and (con2%termal)==0);

		}
		averages(i+1);
		// cout << _global_av(_index_tenergy) << "\n";

		block_reset();
		// cout << _block_av(_index_tenergy) << "\n";
	}
}

// void H :: operator=(const H& h2){
// 	_H=h2.Get_H();
// 	err_H=h2.Get_err_H();
// 	accettazione=h2.Get_accettazione();
// 	u=h2.Get_u();
// 	s=h2.Get_s();

// }

SA::SA() : H_accepted(), H_try(){


	int p1, p2; // Read from ./INPUT/Primes a pair of numbers to be used to initialize the RNG
	ifstream Primes("./INPUT/Primes");
	Primes >> p1 >> p2 ;
	Primes.close();
	int seed[4]; // Read the seed of the RNG
	ifstream Seed("./INPUT/seed.in");
	Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
	_rnd.SetRandom(seed,p1,p2);

	ofstream coutf;
	coutf.open("./OUTPUT/SA.dat");
	coutf << "    #T:              iterazioni:              accettazione_<H>:              mu:              sigma:              E_final:              ERROR:" << endl;
	coutf.close();
	coutf.open("./OUTPUT/SA_acceptance.dat");
	coutf << "    #T:              ACCEPTANCE:" << endl;
	coutf.close();
	ifstream input("./INPUT/SA_input.dat");
	string property;
	double u_i;
	double s_i;
	if (input.is_open()){
		while ( !input.eof() ){
			input >> property;
		if( property == "Initial_Temp" ){
			input >> T_i;
			T=T_i;
		}else if( property == "Final_Temp" ){
			input >> T_f;
			// cerr << T_f << endl;
		}else if( property == "Initial_mu" ){
			input >> u_i;
			H_accepted.Set_u(u_i);
			// cout << H_accepted.Get_u() << endl;
		}else if( property == "Initial_sigma" ){
			input >> s_i;
			H_accepted.Set_s(s_i);
		} else if( property == "DELTA" ){
			input >> delta;
		}else if( property == "N_TEMPERATURE" ){
			input >> n_Temperature;
			if(n_Temperature<=0) cerr << "ERRORE! IMPOSSIBILE AVERE UN NUMERO DI TEMPERATURE NEGATIVO O NULLO!\n";
		}else if( property == "ITERATION_PER_TEMPERATURE" ){
			input >> iterations;
		} else if( property == "ENDINPUT" ){
			break;
		} else cerr << "PROBLEM: unknown input" << endl;
	}
	input.close();
	} else cerr << "PROBLEM: Unable to open SA_input.dat" << endl;

}

H SA::Get_H_accepted(){
	return H_accepted;
}

void SA::SA_metro(){  // vario i parametri a temperatura fissata
	
	H_try=H();

	H_try.Set_u(H_accepted.Get_u()+_rnd.Rannyu(-delta,delta));
	H_try.Set_s(H_accepted.Get_s()+_rnd.Rannyu(-delta,delta));

	H_try.Set_delta(H_accepted.Get_delta()); // mi immagino che avere un delta che risultava ottimale per i parametri vecchi sia un miglior punto di partenza della delta iniziale
	
	// cout << H_accepted.Get_delta() << "\t" << H_accepted.Get_accettazione() << endl;
	
	// cout << "fjsljsl\n" << iterations << endl;

	H_try.H_fin();



	// cout << "fjsljsl\n" << iterations << endl;

	double delta_L=H_try.Get_H()-H_accepted.Get_H();
	// cout << delta_L << endl;
	double acceptance = exp(-(1./T)*delta_L);
	nattempts++;
	if(_rnd.Rannyu() < acceptance){
		H_accepted=H_try;
		naccepted++;
		// cout << naccepted << endl;
	}
}



void SA::minimizza(){
	H_accepted.H_fin();
	double t=0.;
	double Dt;
	if((n_Temperature-1)>0) Dt=1./(n_Temperature-1); // faccio in modo che t all'inizio sia 0 e alla fine sia 1
	if((n_Temperature-1)==0) Dt=0;

	for(int j=0; j<n_Temperature; j++){	
		double fraction;
		naccepted=0;
		nattempts=0;
		// cerr << t << "\t" << T << endl;

		ofstream coutf;
		coutf << scientific;
		coutf.open("./OUTPUT/SA.dat",ios::app);
		coutf << setw(12) << T 
		<< " " << setw(13) << 0
		<< " " << setw(31) << H_accepted.Get_accettazione()
		<< " " << setw(23) << H_accepted.Get_u()
		<< " " << setw(20) << H_accepted.Get_s()
		<< " " << setw(20) << H_accepted.Get_H()
		<< " " << setw(21) << H_accepted.Get_err_H() << endl;
		coutf.close();
		for(int i=0;i<iterations;i++){
			SA_metro();
			coutf.open("./OUTPUT/SA.dat",ios::app);
			coutf << setw(12) << T 
			<< " " << setw(13) << i+1
			<< " " << setw(31) << H_accepted.Get_accettazione()
			<< " " << setw(23) << H_accepted.Get_u()
			<< " " << setw(20) << H_accepted.Get_s()
			<< " " << setw(20) << H_accepted.Get_H()
			<< " " << setw(21) << H_accepted.Get_err_H() << endl;
			coutf.close();

			coutf.open("./OUTPUT/SA_acceptance.dat",ios::app);
			if(nattempts > 0) fraction = double(naccepted)/double(nattempts);
			else fraction = 0.0; 
			if(i+1==iterations){// scrivo alla fine della temperatura l'accettazione di quella temperatura
				coutf << setw(12) << T 
				<< " " << setw(20) << fraction << endl;
			}
			coutf.close();
		}
		t+=Dt;
		// if(j==(n_Temperature-2)) T=T_f;// altrimenti sono a un valore 0.0066*(T_i-T_f) più in alto
		// else{
		T=T_function(t);
		if(T<=0) cerr << "ATTENZIONE! E' STATA INSERITA UNA TEMPERATURA NEGATIVA!\n";
		// }
	}
}

// double SA::T_function(double t){
// 	return 	((T_f-T_i)/(2.*tanh(0.5)))*tanh(t-0.5)+0.5*(T_i+T_f); // vale T_i in 0 e T_f in 1
// }

double SA::T_function(double t){
	return T_i*pow(T_f/T_i,t); // vale T_i in 0 e T_f in 1
}