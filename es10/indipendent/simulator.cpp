#include <iostream>
#include <string>
#include <fstream>
#include <iomanip> //setw()
#include <vector>
#include <mpi.h>
#include <cmath>
#include "TSP.hpp"
#include "random.h"

using namespace std;

double distance_L1(const vector<double>& a, const vector<double>& b){
	if(a.size()!=b.size()){
		cerr << "ERRORE! IMPOSSIBILE CALCOLARE LA DISTANZA TRA VETTORI CON DIMENSIONI DIVERSE!\n";
	}
	double dist=0;
	for(int i=0; i<a.size(); i++){
		dist+=pow((a[i]-b[i]),2);
	}
	dist=sqrt(dist);

	// double dist = std::accumulate(p.begin(), p.end(), 0.0, [](double acc, double val) {
    // 		return acc + abs(val);
	// 	}// usata lambda function
	// );
	return dist;
}


double distance_L2(const vector<double>& a, const vector<double>& b){
	if(a.size()!=b.size()){
		cerr << "ERRORE! IMPOSSIBILE CALCOLARE LA DISTANZA TRA VETTORI CON DIMENSIONI DIVERSE!\n";
	}
	double dist=0;
	for(int i=0; i<a.size(); i++){
		dist+=pow((a[i]-b[i]),2);
	}

	// double dist = std::accumulate(p.begin(), p.end(), 0.0, [](double acc, double val) {
    // 		return acc + abs(val);
	// 	}// usata lambda function
	// );
	return dist;
}

// distanza su sfera di raggio 1 a partire da phi e theta (in gradi e in questo ordine)
double distance_L1_sph(const vector<double>& a, const vector<double>& b){
	if(a.size()!=2 or b.size()!=2){
		cerr << "ERRORE! LA FUNZIONE: \"distance_L1_sph\" NECESSITA DI DUE VETTORI CON DIMENSIONE 2\n";
		return -1.;
	}
	if(a[0]<-180. or b[0]<-180. or a[0]>=180. or b[0]>=180. or a[1]<-90. or b[1]<-90. or a[1]>=90. or b[1]>=90.){ // nonostante il nome M_PI non viene dalla libreria MPI ma è pigreco e si trova in cmath 
		cerr << "ERRORE! LA FUNZIONE: \"distance_L1_sph\" NECESSITA DI VETTORI CON COMPONENTI: phi: [-180,180), theta: [-90, 90)\n";
		return -1.0;
	}
	double dist=acos(sin(a[1]*M_PI/180.)*sin(b[1]*M_PI/180.)+cos(a[1]*M_PI/180.)*cos(b[1]*M_PI/180.)*cos((b[0]-a[0])*M_PI/180.));

	return dist;
}

double distance_L2_sph(const vector<double>& a, const vector<double>& b){
	if(a.size()!=2 or b.size()!=2){
		cerr << "ERRORE! LA FUNZIONE: \"distance_L2_sph\" NECESSITA DI DUE VETTORI CON DIMENSIONE 2\n";
		return -1.;
	}
	if(a[0]<-180. or b[0]<-180. or a[0]>=180. or b[0]>=180. or a[1]<-90. or b[1]<-90. or a[1]>=90. or b[1]>=90.){ 
		cerr << "ERRORE! LA FUNZIONE: \"distance_L2_sph\" NECESSITA DI VETTORI CON COMPONENTI: phi: [-180,180), theta: [-90, 90)\n";
		return -1.0;
	}
	double dist=acos(sin(a[1]*M_PI/180.)*sin(b[1]*M_PI/180.)+cos(a[1]*M_PI/180.)*cos(b[1]*M_PI/180.)*cos((b[0]-a[0])*M_PI/180.));

	return pow(dist,2);
}

int main (int argc, char *argv[]){
	
	// cerr << fixed << setprecision(5) << 6370.97*distance_L1_sph({12.496366,41.902782},{-73.935242,40.730610}) << endl; // distanza Roma-NewYork

	bool square=true;
	bool circ=true;
	bool L1=true;
	bool L2=true;

	int size, rank;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status stat;


	ofstream ev;
	ofstream best;
	std::function<double(vector<double>& , const vector<double>& )> norm;
	string dist;
	string shape;
	double sum;
	int n_for_av;
	TSP sistema;
	string input_file_TSP;
	string nodes="_core"+to_string(rank);


	if(L1){
		norm =distance_L1_sph;
		dist = "L1";
	
		input_file_TSP="./INPUT/CAP_PROV_ITA/input_simulator.in";
		sistema=TSP(rank + 1, input_file_TSP, "./INPUT/cap_prov_ita.dat", norm);
		
		n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
		ev.open("./OUTPUT/"+dist+"/evolution"+nodes+".out");
		best.open("./OUTPUT/"+dist+"/best_travel"+nodes+".out");
		ev << setprecision(16);
		best << setprecision(16);
		ev << "    # generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
		for(int i=0; i<sistema.Get_n_generations(); i++){
			sum=0;
			sistema.new_generation();
			sistema.sorting();

			for(int s=0; s<n_for_av; s++){
				sum+=sistema.V_individuals[s].loss_function;
			}
			sum/=double(n_for_av); // diviene una media
			ev << setw(12) << i+1 << setw(31) <<  sistema.V_individuals[0].loss_function
				<< " " << setw(30) << sum << endl; 

			if(i==(sistema.Get_n_generations()-1)){
				for (int s=0; s<(sistema.V_individuals[0]).n_cities; s++) {
					best << sistema.V_individuals[0].cities[s] << " ";
				}
				best << endl;
			}
		}
		ev.close();
		best.close();
	}

	if(L2){
		norm=distance_L2_sph;
		dist ="L2";

		input_file_TSP="./INPUT/CAP_PROV_ITA/input_simulator.in";
		sistema=TSP(rank + 1, input_file_TSP, "./INPUT/cap_prov_ita.dat", norm);
		
		n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
		ev.open("./OUTPUT/"+dist+"/evolution"+nodes+".out");
		best.open("./OUTPUT/"+dist+"/best_travel"+nodes+".out");
		ev << setprecision(16);
		best << setprecision(16);
		ev << "    # generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
		for(int i=0; i<sistema.Get_n_generations(); i++){
			sum=0;
			sistema.new_generation();
			sistema.sorting();

			for(int s=0; s<n_for_av; s++){
				sum+=sistema.V_individuals[s].loss_function;
			}
			sum/=double(n_for_av); // diviene una media
			ev << setw(12) << i+1 << setw(31) <<  sistema.V_individuals[0].loss_function
				<< " " << setw(30) << sum << endl; 

			if(i==(sistema.Get_n_generations()-1)){
				for (int s=0; s<(sistema.V_individuals[0]).n_cities; s++) {
					best << sistema.V_individuals[0].cities[s] << " ";
				}
				best << endl;
			}
		}
		ev.close();
		best.close();
	} 
	

	// if(L1){
	// 	norm =distance_L1;
	// 	dist ="L1/";
	// 	if(circ){
	// 		shape="circumference";

	// 		input_file_TSP="./INPUT/"+dist+shape+"/input_simulator.in";
			
	// 		sistema=TSP(rank + 1, input_file_TSP, "./INPUT/"+shape+".in", norm);
	// 		n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
	// 		ev.open("./OUTPUT/"+dist+shape+"/evolution"+nodes+".out");
	// 		best.open("./OUTPUT/"+dist+shape+"/best_travel"+nodes+".out");
	// 		ev << "    # generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
	// 		for(int i=0; i<sistema.Get_n_generations(); i++){
	// 			sum=0;
	// 			sistema.new_generation();
	// 			sistema.sorting();

	// 			for(int s=0; s<n_for_av; s++){
	// 				sum+=sistema.V_individuals[s].loss_function;
	// 			}
	// 			sum/=double(n_for_av); // diviene una media
	// 			ev << setw(12) << i+1 << setw(26) <<  sistema.V_individuals[0].loss_function
	// 				<< " " << setw(30) << sum << endl; 

	// 			if(i==(sistema.Get_n_generations()-1)){
	// 				for (int s=0; s<(sistema.V_individuals[0]).n_cities; s++) {
	// 					best << sistema.V_individuals[0].cities[s] << " ";
	// 				}
	// 				best << endl;
	// 			}
	// 		}
	// 		ev.close();
	// 		best.close();
	// 	}

	// 	if(square){
	// 		shape="square";
	// 		input_file_TSP="./INPUT/"+dist+shape+"/input_simulator.in";
	// 		sistema=TSP(rank + 1, input_file_TSP, "./INPUT/"+shape+".in", norm);
			
	// 		n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
	// 		ev.open("./OUTPUT/"+dist+shape+"/evolution"+nodes+".out");
	// 		best.open("./OUTPUT/"+dist+shape+"/best_travel"+nodes+".out");
	// 		ev << "    # generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
	// 		for(int i=0; i<sistema.Get_n_generations(); i++){
	// 			sum=0;
	// 			sistema.new_generation();
	// 			sistema.sorting();

	// 			for(int s=0; s<n_for_av; s++){
	// 				sum+=sistema.V_individuals[s].loss_function;
	// 			}
	// 			sum/=double(n_for_av); // diviene una media
	// 			ev << setw(12) << i+1 << setw(26) <<  sistema.V_individuals[0].loss_function
	// 				<< " " << setw(30) << sum << endl; 

	// 			if(i==(sistema.Get_n_generations()-1)){
	// 				for (int s=0; s<(sistema.V_individuals[0]).n_cities; s++) {
	// 					best << sistema.V_individuals[0].cities[s] << " ";
	// 				}
	// 				best << endl;
	// 			}
	// 		}
	// 		ev.close();
	// 		best.close();
	// 	}
	// }
	
	// if(L2){
	// 	norm=distance_L2;
	// 	dist ="L2/";

	// 	if(circ){
	// 		shape="circumference";
	// 		input_file_TSP="./INPUT/"+dist+shape+"/input_simulator.in";
	// 		sistema=TSP(rank + 1, input_file_TSP, "./INPUT/"+shape+".in", norm);
			
	// 		n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
	// 		ev.open("./OUTPUT/"+dist+shape+"/evolution"+nodes+".out");
	// 		best.open("./OUTPUT/"+dist+shape+"/best_travel"+nodes+".out");
	// 		ev << "    # generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
	// 		for(int i=0; i<sistema.Get_n_generations(); i++){
	// 			sum=0;
	// 			sistema.new_generation();
	// 			sistema.sorting();

	// 			for(int s=0; s<n_for_av; s++){
	// 				sum+=sistema.V_individuals[s].loss_function;
	// 			}
	// 			sum/=double(n_for_av); // diviene una media
	// 			ev << setw(12) << i+1 << setw(26) <<  sistema.V_individuals[0].loss_function
	// 				<< " " << setw(30) << sum << endl; 

	// 			if(i==(sistema.Get_n_generations()-1)){
	// 				for (int s=0; s<(sistema.V_individuals[0]).n_cities; s++) {
	// 					best << sistema.V_individuals[0].cities[s] << " ";
	// 				}
	// 				best << endl;
	// 			}
	// 		}
	// 		ev.close();
	// 		best.close();
	// 	}

	// 	if(square){
	// 		shape="square";
	// 		input_file_TSP="./INPUT/"+dist+shape+"/input_simulator.in";
	// 		sistema=TSP(rank + 1, input_file_TSP, "./INPUT/"+shape+".in", norm);
			
	// 		n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
	// 		ev.open("./OUTPUT/"+dist+shape+"/evolution"+nodes+".out");
	// 		best.open("./OUTPUT/"+dist+shape+"/best_travel"+nodes+".out");
	// 		ev << "    # generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
	// 		for(int i=0; i<sistema.Get_n_generations(); i++){
	// 			sum=0;
	// 			sistema.new_generation();
	// 			sistema.sorting();

	// 			for(int s=0; s<n_for_av; s++){
	// 				sum+=sistema.V_individuals[s].loss_function;
	// 			}
	// 			sum/=double(n_for_av); // diviene una media
	// 			ev << setw(12) << i+1 << setw(26) <<  sistema.V_individuals[0].loss_function
	// 				<< " " << setw(30) << sum << endl; 

	// 			if(i==(sistema.Get_n_generations()-1)){
	// 				for (int s=0; s<(sistema.V_individuals[0]).n_cities; s++) {
	// 					best << sistema.V_individuals[0].cities[s] << " ";
	// 				}
	// 				best << endl;
	// 			}
	// 		}
	// 		ev.close();
	// 		best.close();
	// 	}
		
	// } 
	MPI_Finalize();


	return 0;
}


