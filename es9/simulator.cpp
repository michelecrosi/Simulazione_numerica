#include <iostream>
#include <string>
#include <fstream>
#include <iomanip> //setw()
#include <vector>
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

int main (int argc, char *argv[]){
	bool square=true;
	bool circ=true;
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


	if(L1){
		norm =distance_L1;
		dist ="L1/";
		if(circ){
			shape="circumference";

			input_file_TSP="./INPUT/"+dist+shape+"/input_simulator.in";
			
			sistema=TSP(input_file_TSP, "./INPUT/"+shape+".in", norm);
			n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
			ev.open("./OUTPUT/"+dist+shape+"/evolution.out");
			best.open("./OUTPUT/"+dist+shape+"/best_travel.out");
			ev << scientific;
			best << scientific;
			ev << "    #generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
			for(int i=0; i<sistema.Get_n_generations(); i++){
				sum=0;
				sistema.new_generation();
				sistema.sorting();

				for(int s=0; s<n_for_av; s++){
					sum+=sistema.V_individuals[s].loss_function;
				}
				sum/=double(n_for_av); // diviene una media
				ev << setw(12) << i+1 << setw(26) <<  sistema.V_individuals[0].loss_function
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

		if(square){
			shape="square";
			input_file_TSP="./INPUT/"+dist+shape+"/input_simulator.in";
			sistema=TSP(input_file_TSP, "./INPUT/"+shape+".in", norm);
			
			n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
			ev.open("./OUTPUT/"+dist+shape+"/evolution.out");
			best.open("./OUTPUT/"+dist+shape+"/best_travel.out");
			ev << scientific;
			best << scientific;
			ev << "    #generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
			for(int i=0; i<sistema.Get_n_generations(); i++){
				sum=0;
				sistema.new_generation();
				sistema.sorting();

				for(int s=0; s<n_for_av; s++){
					sum+=sistema.V_individuals[s].loss_function;
				}
				sum/=double(n_for_av); // diviene una media
				ev << setw(12) << i+1 << setw(26) <<  sistema.V_individuals[0].loss_function
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
	}
	


	if(L2){
		norm=distance_L2;
		dist ="L2/";

		if(circ){
			shape="circumference";
			input_file_TSP="./INPUT/"+dist+shape+"/input_simulator.in";
			sistema=TSP(input_file_TSP, "./INPUT/"+shape+".in", norm);
			
			n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
			ev.open("./OUTPUT/"+dist+shape+"/evolution.out");
			best.open("./OUTPUT/"+dist+shape+"/best_travel.out");
			ev << scientific;
			best << scientific;
			ev << "    #generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
			for(int i=0; i<sistema.Get_n_generations(); i++){
				sum=0;
				sistema.new_generation();
				sistema.sorting();

				for(int s=0; s<n_for_av; s++){
					sum+=sistema.V_individuals[s].loss_function;
				}
				sum/=double(n_for_av); // diviene una media
				ev << setw(12) << i+1 << setw(26) <<  sistema.V_individuals[0].loss_function
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

		if(square){
			shape="square";
			input_file_TSP="./INPUT/"+dist+shape+"/input_simulator.in";
			sistema=TSP(input_file_TSP, "./INPUT/"+shape+".in", norm);
			
			n_for_av=int(sistema.Get_n_individuals()*0.5+0.9); // arrotondo per eccesso
			ev.open("./OUTPUT/"+dist+shape+"/evolution.out");
			best.open("./OUTPUT/"+dist+shape+"/best_travel.out");
			ev << scientific;
			best << scientific;
			ev << "    #generation:          shortest_distance:          average_of_top_" << n_for_av << ":" << endl;
			for(int i=0; i<sistema.Get_n_generations(); i++){
				sum=0;
				sistema.new_generation();
				sistema.sorting();

				for(int s=0; s<n_for_av; s++){
					sum+=sistema.V_individuals[s].loss_function;
				}
				sum/=double(n_for_av); // diviene una media
				ev << setw(12) << i+1 << setw(26) <<  sistema.V_individuals[0].loss_function
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
		
	} 







	// for(int j=0; j<sistema.Get_n_individuals(); j++){
	// 	if(!sistema.V_individuals[j].check_f()) cerr << "ERRORE!\n";

	// 	std::cout << "V_" << j << ": ";
	// 	for (const auto& elem : (sistema.V_individuals[j]).cities) {
	// 		std::cout << elem << " ";
	// 	}
	// 	std::cout << std::endl;
	// }








	// // test operatori (e insert):

	// _operator muta;
	// vector<int> V = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	// std::cout << "V: ";
    // for (const auto& elem : V) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;


	
	// // insert sembra funzionare

	// insert(V, 1, 7);

	// std::cout << "V: ";
    // for (const auto& elem : V) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;





	// // swap sembra funzionare

	// muta.swap(V,1,3);
	// std::cout << "V: ";
    // for (const auto& elem : V) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;






	// //shift sembra funzionare

	// // muta.shift(V, 6, 2, 1);
	// // muta.shift(V, 2, 3, 3);
	// // muta.shift(V, 1, 7, 4);
	// muta.shift(V, 2, 2, 4);


	// std::cout << "V: ";
    // for (const auto& elem : V) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;
	





	// // block_swap sembra funzionare
	
	// // muta.block_swap(V,1,3,5);
	// // muta.block_swap(V,7,3,2);
	// muta.block_swap(V,6,4,2);
	// std::cout << "V: ";
    // for (const auto& elem : V) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;






	// // inversion sembra funzionare
	
	// muta.inversion(V,5,8);
	// std::cout << "V: ";
    // for (const auto& elem : V) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;






// 	// il crossing sembra funzionare

// 	std::vector<int> V = {2, 1, 3, 4, 5};
//     std::vector<int> W = {5, 4, 3, 2, 1};
//     int cut_index = 2;

// 	std::cout << "V: ";
//     for (const auto& elem : V) {
//         std::cout << elem << " ";
//     }
//     std::cout << std::endl;

//     std::cout << "W: ";
//     for (const auto& elem : W) {
//         std::cout << elem << " ";
//     }
//     std::cout << std::endl;

//     _operator cr;
//     cr.cross(V, W, cut_index);

//     std::cout << "V: ";
//     for (const auto& elem : V) {
//         std::cout << elem << " ";
//     }
//     std::cout << std::endl;

//     std::cout << "W: ";
//     for (const auto& elem : W) {
//         std::cout << elem << " ";
//     }
//     std::cout << std::endl;

	return 0;
}


