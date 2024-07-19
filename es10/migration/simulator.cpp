#include <iostream>
#include <string>
#include <fstream>
#include <iomanip> //setw()
#include <vector>
#include <mpi.h>
#include "TSP.hpp"
#include "random.h"

using namespace std;


// class Individual {
// public:
//     std::vector<int> cities;
//     double loss_function;

//     Individual() : cities(), loss_function(0) {}
// };

// int main(int argc, char* argv[]) {
//     MPI_Init(&argc, &argv);
    
//     int rank, size;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);
    
//     std::vector<individual> v_individuals;
//     v_individuals.push_back(individual()); // Aggiungi almeno un individuo

//     std::vector<int> copy_to_send = {1, 2, 3, 4}; // Esempio di dati da inviare
//     double copy_to_send_loss = 1.0;
//     std::vector<int> copy_to_get(copy_to_send.size());
//     double copy_to_get_loss;

//     // int dest_rank = (rank + 1) % size;
//     // int source_rank = (rank - 1 + size) % size;

//     // if (rank == 0) {
//     //     MPI_Send(copy_to_send.data(), copy_to_send.size(), MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
//     //     MPI_Send(&copy_to_send_loss, 1, MPI_DOUBLE, dest_rank, 1, MPI_COMM_WORLD);
//     // } 
// 	// if (rank == 1){
//     //     MPI_Recv(copy_to_get.data(), copy_to_get.size(), MPI_INT, source_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//     //     MPI_Recv(&copy_to_get_loss, 1, MPI_DOUBLE, source_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//     // }

//     if (rank != 0) {
//         v_individuals[0].cities = copy_to_get;
//         v_individuals[0].loss_function = copy_to_get_loss;
//     }

//     if (rank != 0) {
//         for (int city : v_individuals[0].cities) {
//             std::cout << city << " ";
//         }
//         std::cout << std::endl;
//     }

//     MPI_Finalize();
//     return 0;
// }

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



	MPI_Init(&argc,&argv);
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status stat;

	
	if(size!=8) cerr << "AVVERTENZA! Questo programma è pensato per usare 8 nodi\n";

	if(size%2 != 0) size--;// visto che devo fare coppie che si scambiano i dati faccio in modo di non considerare l'ultimo nodo se i nodi sono dispari

	// if(rank != 0){
	// 	Random ran;
	// 	vector<vector<int>> per;
	// 	int n_estra=7;
	// 	for(int k=0; k<n_estra; k++){
	// 		per=ran.couple_perm(8);
	// 		for(int i=0; i<per.size(); i++){
	// 			cerr << "rango: " << rank << "\t";
	// 			for(int j=0;j<(per[0]).size();j++){
	// 				cerr << per[i][j] << "\t";
	// 			} 
	// 			cerr << endl;
	// 		}
	// 		cerr << endl;
	// 	}
	// }
	
	
	bool L1=true;
	bool L2=true;
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
	Random random; // random che ha lo stesso seme in ogni nodo, usata per generare le coppie che fanno gli scambi
	vector<vector<int>> couples;
	vector<int> copy_to_send; // vettore che uso per passare le città fra nodi diversi
	vector<int> copy_to_get; // vettore che uso per ricevere le città fra nodi diversi
	double copy_to_get_loss; // uso per passare loss function fra nodi diversi
	double copy_to_send_loss; // uso per ricevere loss function fra nodi diversi
	
	if(L1){
		
		norm =distance_L1_sph;
		dist = "L1";
	
		input_file_TSP="./INPUT/CAP_PROV_ITA/input_simulator.in";
		sistema=TSP(rank + 1, input_file_TSP, "./INPUT/cap_prov_ita.dat", norm);
		
		copy_to_get=vector<int>((sistema.V_individuals[0]).n_cities);


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


			if(i % sistema.Get_N_migr() == (sistema.Get_N_migr()-1)){
				// cerr << i << endl;
				// vector<vector<int>> couples (size/2, vector<int>(2, 0));
				couples=random.couple_perm(size);
				copy_to_send=sistema.V_individuals[0].cities;
				copy_to_send_loss=sistema.V_individuals[0].loss_function;

				for(int j=0; j<couples.size(); j++){
					for(int k=0; k<(couples[0]).size(); k++){
						if( rank==couples[j][k]){
							if(k==0){								
								MPI_Send(&copy_to_send[0], copy_to_send.size(), MPI_INTEGER, couples[j][1], j, MPI_COMM_WORLD);// mandato un vettore								
								MPI_Send(&copy_to_send_loss, 1, MPI_DOUBLE, couples[j][1], j+couples.size(), MPI_COMM_WORLD);// mandato un double
								MPI_Recv(&copy_to_get[0], copy_to_send.size(), MPI_INTEGER, couples[j][1], j, MPI_COMM_WORLD, &stat);
								MPI_Recv(&copy_to_get_loss, 1, MPI_DOUBLE, couples[j][1], j+couples.size(), MPI_COMM_WORLD, &stat);
							}
							if(k==1){
								MPI_Recv(&copy_to_get[0], copy_to_send.size(), MPI_INTEGER, couples[j][0], j, MPI_COMM_WORLD, &stat);
								MPI_Recv(&copy_to_get_loss, 1, MPI_DOUBLE, couples[j][0], j+couples.size(), MPI_COMM_WORLD, &stat);
								MPI_Send(&copy_to_send[0], copy_to_send.size(), MPI_INTEGER, couples[j][0], j, MPI_COMM_WORLD);
								MPI_Send(&copy_to_send_loss, 1, MPI_DOUBLE, couples[j][0], j+couples.size(), MPI_COMM_WORLD);
							}
						}
					}
				}
	
				// for(int j=0; j<sistema.V_individuals[0].cities.size(); j++){
				// 	(sistema.V_individuals[0]).cities[j]=copy_to_get[j];
				// }
				(sistema.V_individuals[0]).cities=copy_to_get;
				// cerr << copy_to_send[1] << "\t" << copy_to_get[1] << "\n";
				// cerr << copy_to_send_loss << "\t" << copy_to_get_loss << "\t" << (sistema.V_individuals[0]).loss_function << "\n";
				(sistema.V_individuals[0]).loss_function=copy_to_get_loss;
				sistema.sorting();
			}

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

			if(i % sistema.Get_N_migr() == (sistema.Get_N_migr()-1)){
				couples=random.couple_perm(size);
				copy_to_send=sistema.V_individuals[0].cities;
				copy_to_send_loss=sistema.V_individuals[0].loss_function;

				for(int j=0; j<couples.size(); j++){
					for(int k=0; k<(couples[0]).size(); k++){
						if( rank==couples[j][k]){
							if(k==0){								
								MPI_Send(&copy_to_send[0], copy_to_send.size(), MPI_INTEGER, couples[j][1], j, MPI_COMM_WORLD);// mandato un vettore								
								MPI_Send(&copy_to_send_loss, 1, MPI_DOUBLE, couples[j][1], j+couples.size(), MPI_COMM_WORLD);// mandato un double
								MPI_Recv(&copy_to_get[0], copy_to_send.size(), MPI_INTEGER, couples[j][1], j, MPI_COMM_WORLD, &stat);
								MPI_Recv(&copy_to_get_loss, 1, MPI_DOUBLE, couples[j][1], j+couples.size(), MPI_COMM_WORLD, &stat);
							}
							if(k==1){
								MPI_Recv(&copy_to_get[0], copy_to_send.size(), MPI_INTEGER, couples[j][0], j, MPI_COMM_WORLD, &stat);
								MPI_Recv(&copy_to_get_loss, 1, MPI_DOUBLE, couples[j][0], j+couples.size(), MPI_COMM_WORLD, &stat);
								MPI_Send(&copy_to_send[0], copy_to_send.size(), MPI_INTEGER, couples[j][0], j, MPI_COMM_WORLD);
								MPI_Send(&copy_to_send_loss, 1, MPI_DOUBLE, couples[j][0], j+couples.size(), MPI_COMM_WORLD);
							}
						}
					}
				}
	
				(sistema.V_individuals[0]).cities=copy_to_get;
				(sistema.V_individuals[0]).loss_function=copy_to_get_loss;
				sistema.sorting();
			}

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
	MPI_Finalize();

	
	return 0;
}


