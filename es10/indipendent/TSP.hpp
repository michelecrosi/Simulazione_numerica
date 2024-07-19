#ifndef __TSP__
#define __TSP__

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <algorithm> // uso find() e std::sort() e std::min()
#include <iterator> // Per std::find
#include <functional> // Includi l'header per std::function
#include <vector>

#include "random.h"

using namespace std;


// class city{
// 	public:
// 		city();
// 		int n; // intero che identifica la città
// 		vector<double> pos();

// };


int periodic_condition(const int& index, const int& dim_vec);//mi restituisce l'indice ottenuto con l'effetto pac-man se sforo, ignora il primo elemento del vettore



class _operator{
	public:
		_operator();
		_operator(int riga);

		Random rnd;


		//selection

		template <typename T>
		T select(vector<T>& V); // estrae un elemento casuale dal vettore con probabilità più elevata per quelli con indici più piccoli


		// mutations

		template <typename T>
		void swap(vector<T>& V, const int& initial_index1, const int& initial_index2);
		
		template <typename T>
		void shift(vector<T>& V, const int& initial_index, const int& m, const int& n_shift );// n_shift è la dimensione della traslazione mentre 
		// per come è fatta la mutazione NON posso avere l'effetto pac-man

		template <typename T>
		void block_swap(vector<T>& V, const int& initial_index1, const int& m, const int& initial_index2); // m è il numero di elementi dei blocchi che considero
		// per come è fatta la mutazione posso avere l'effetto pac-man (ignorando il primo elemento)

		template <typename T>
		void inversion(vector<T>& V, const int& m, const int& initial_index);// m è il numero di elementi dei blocchi che considero
		// per come è fatta la mutazione non posso avere l'effetto pac-man (ignorando il primo elemento)


		// crossing

		template <typename T>
		void cross(vector<T>& V, vector<T>& W, const int& cut_index); // cut index è l'indice dopo il quale faccio il taglio

	protected:
		double esponent; // esponente nella selezione

};

class individual{
	public:
		individual();
		individual(int num_cities);
		bool check_f();// mi restituisce false se ho delle incongruenze
		double loss_function;
		vector<int> cities; // sono in ordine di percorrenza sottointendendo il ritorno all'inizio e senza scrivere la città iniziale
		int n_cities;
};

class TSP{
	public:
		TSP(){};
		TSP(string input_file_TSP, string input_file_cities, std::function<double(vector<double>& , const vector<double>& )> input_distance) ;// do il nome del file con le posizione delle città
		TSP(int riga, string input_file_TSP, string input_file_cities, std::function<double(vector<double>& , const vector<double>& )> input_distance) ;// do il nome del file con le posizione delle città
		
		vector<individual> V_individuals; //popolazione
		vector<individual> V_sons; // figli
		void sorting(); // ordino in ordine crescente di loss function dopo aver calcolate queste
		void new_generation();
		int Get_n_generations();
		int Get_n_individuals();
	protected:
		bool sorted; // ho true se V_individuals è stato sortato
		vector<vector<double>> M_distances; //matrice delle distanze (elemento i-j è distanza tra la città i+1 e la città j+1)
		_operator op;
		double P_m; //probabilità di avere mutazioni
		double P_c; //probabilità di crossover
		int n_individuals;
		int n_generations;
		std::function<double(vector<double>& , const vector<double>& )> distance;
};








/////////// TEMPLATES /////////////////////

// template <typename T>
// void min(const T& a, const T& b){
// 	if(a>b) return b;
// 	else return a;
// }

template <typename T>
void insert(vector<T>& V, const int& moved_index, const int& surpassed_index){
	// moved_index=Indice dell'elemento da spostare
    // surpassed_index=indice che nel vettore iniziale indica l'elemento dopo il quale inserirò l'elemento traslato

	if (moved_index < 0 or moved_index >= V.size() or surpassed_index < 0 or surpassed_index >= V.size()) {
        throw std::out_of_range("Index out of range in function: 'insert'");
    }
    
    // Elemento da spostare
    T elementToMove = V[moved_index];

	// Rimuovo l'elemento dal suo indice originale
    V.erase(V.begin() + moved_index);
	
    // Inserisco l'elemento nel nuovo indice
    if(surpassed_index >= moved_index) V.insert(V.begin() + surpassed_index, elementToMove); 
	else V.insert(V.begin() + surpassed_index+1, elementToMove);// aggiungo 1 perchè alla funzione do l'indice dell'elemento che voglio aggiungere
} // se surpassed_index==moved_index non cambia nulla




template <typename T>
T _operator::select(vector<T>& V){
	return V[int(V.size()*pow(rnd.Rannyu(),esponent))];
}



template <typename T>
void _operator::swap(vector<T>& V, const int& initial_index1, const int& initial_index2){
	block_swap(V, initial_index1, 1, initial_index2);
}

template <typename T>
void _operator::shift(vector<T>& V, const int& initial_index, const int& m, const int& n_shift){
	if(initial_index<=0){
		cerr << "ERRORE! nelle mutazioni gli indici devono essere positivi\n";
		return;
	} 
	if(m<=0){
		cerr << "ERRORE! la lunghezza del blocco deve essere positiva\n";
		return;
	} 
	if(n_shift<=0){
		cerr << "ERRORE! il numero di blocchi di cui shifto deve essere positivo\n";
		return;
	} 
	if((m+initial_index+n_shift)>V.size()){
		cerr << "ERRORE! Nella mutazione: 'shift' il blocco da tralsare supera il range del vettore \n";
		return;
	} 

	for(int i=0; i<m; i++){
		insert(V, initial_index, initial_index+m-1+n_shift );
	}

	// int ind_before_insert;
	// int ind_from;
	// int d=0;
	// for(int i=0; i<m; i++){
	// 	ind_from=initial_index+d;
	// 	ind_before_insert=periodic_condition(initial_index+m-1+n_shift+d, V.size());
	// 	insert(V, ind_from, ind_before_insert );
	// 	if(ind_before_insert<=ind_from) d++;
	// }
}

template <typename T>
void _operator::block_swap(vector<T>& V, const int& initial_index1, const int& m, const int& initial_index2 ){
	// cerr << periodic_condition(initial_index2+m-1, V.size()) << endl;

	if(initial_index1<=0 or initial_index1>=V.size() or initial_index2<=0 or initial_index2>=V.size()){
		cerr << "ERRORE! nelle mutazioni gli indici devono essere nel dominio: [1, (dimensione vettore - 1)]\n";
		return;
	} 
	if(m<=0){
		cerr << "ERRORE! la lunghezza del blocco deve essere positiva\n";
		return;
	} 
	if(m>(int((V.size()-1)/2.))){
		cerr << "ERRORE! LA MUTAZIONE: 'block_swap' NON PERMETTE DI AVERE UN BLOCCO MAGGIORE DI: floor(dimensione vettore -1) /2\n";
		return;
	} 
	if((periodic_condition(initial_index1+m-1, V.size())>=initial_index2 && periodic_condition(initial_index1+m-1, V.size())<=periodic_condition(initial_index2+m-1, V.size())) or (periodic_condition(initial_index2+m-1, V.size()) >= initial_index1 && periodic_condition(initial_index2+m-1, V.size()) <= periodic_condition(initial_index1+m-1, V.size()))){
		cerr << "ERRORE! NON POSSO SCAMBIARE BLOCCHI CHE SI SOVRAPPONGONO\n";
		return;
	} 
	for(int i=0; i<m; i++){
		std::swap( V[ periodic_condition(initial_index1+i, V.size()) ], V[periodic_condition(initial_index2+i, V.size())] );
	}

}

template <typename T>
void _operator::inversion(vector<T>& V, const int& initial_index, const int& m){
	if(initial_index<=0 or initial_index>=V.size()){
		cerr << "ERRORE! nelle mutazioni gli indici devono essere nel dominio: [1, (dimensione vettore - 1)]\n";
		return;
	} 
	if(m<=1){
		cerr << "ERRORE! la lunghezza del blocco deve essere positiva\n";
		return;
	}
	if(m>(V.size()-1)){
		cerr << "ERRORE! LA MUTAZIONE: 'inversion' NON PERMETTE DI AVERE UN BLOCCO MAGGIORE DI: (dimensione vettore -1)\n";
		return;
	} 
	for(int i=0; i<int(m/2.); i++){ // sfruttato il fatto che mi casti al floor
		std::swap(V[periodic_condition(initial_index+i, V.size())],V[periodic_condition(initial_index+m-1-i, V.size())]);
	}
}

template <typename T>
void _operator::cross(vector<T>& V, vector<T>& W, const int& cut_index){
	if(V.size()!=W.size()){
		cerr << "ERRORE! non posso fare crossing tra vettori con dimensione diversa\n";
		return;
	} 
	if(cut_index<=0 or cut_index>=(V.size()-1)){
		cerr << "ERRORE! il parametro: 'cut_index' in: 'cross()' deve essere nel dominio: [ 1 , dimensione vettore - 2 ]\n";
		return;
	} 
	// escludo 0 perchè manterrebbe inalterata la coppia (la switcha soltanto)

	vector<T> V_temp(W); // creata copia di W
	for(int i=0; i<=cut_index; i++){
		// devo aggiungere typename sennò ho errore
		typename std::vector<T>::iterator it =std::find(V_temp.begin(),V_temp.end(), V[i]); // iteratore con la posizione che contiene l'elemento V[i] in V_temp
		if (it != V_temp.end()){
			V_temp.erase(it); // lo elimino dal vettore temporaneo
		}
	}
	for(int i=0; i<V_temp.size(); i++){
		V[cut_index+1+i]=V_temp[i];
	}

	V_temp=V; // copia di V
	for(int i=0; i<=cut_index; i++){
		typename std::vector<T>::iterator it =std::find(V_temp.begin(),V_temp.end(), W[i]); // iteratore con la posizione che contiene l'elemento W[i] in V_temp
		if (it != V_temp.end()){
			V_temp.erase(it);
		}
	}
	for(int i=0; i<V_temp.size(); i++){
		W[cut_index+i+1]=V_temp[i];
	}

}	




#endif