#include "TSP.hpp"


using namespace std;




individual::individual(){
	
}

individual::individual(int num_cities){
	n_cities=num_cities;
	cities= vector<int>(num_cities);
	for(int i=0;i<num_cities;i++){
		cities[i]=i+1; // all'inizio le città sono percorse nel senso in cui sono state associate agli interi
	}
}

bool individual::check_f(){
	if(cities[0]!=1) return false;
	for(int i=0; i<cities.size(); i++){
		for(int j=i+1; j<cities.size(); j++){
			if (cities[j] == cities[i]){
				// cerr << i << "\t" << j << endl;
				return false;
			}// controllo quelli dopo perchè quelli prima li ho già controllati
		}
	}
	return true;
}

// v.end() rappresenta un iteratore che punta oltre l'ultimo elemento del vettore


_operator::_operator() : rnd(){
	esponent=2.; // messo maggiore di 1 per avere maggiore estrazione vicino a 0
	// è l'esponente usato per la selezione
}






// double distanza_L2(city a, city b){
// 	vector<double> p=a.pos-b.pos;
// 	return inner_product(p.begin(), p.end(), p.begin(), 0.0); // 0.0 è il valore iniziale dell'accumulatore
// }

// vector<double> city::pos(vector<vector<double>>& traduzione){ // traduzione è il vettore con le città nel numero corrispondente all'intero a cui corrispondono -1
// 	return traduzione(n-1); // prendendo il vettore di vettori dal main così non diviene inefficente
// }// traduzione lo devo definire nel main

int periodic_condition(const int& index, const int& dim_vec){ 
	if(dim_vec<=1){
		cerr << "ERRORE! il parametro: 'dim_vec' in: 'periodic_condition()' deve essere > 1\n";
		return -1;
	} 
	if(index<=0 or index>2*(dim_vec-1)){
		cerr << "ERRORE! il parametro: 'index' in: 'periodic_condition()' deve essere nel dominio: [ 1 , 2*(dimensione vettore -1) ]\n";
		return -2;
	} 
	if(index<=dim_vec-1) return index;
	return (((index-1) % (dim_vec-1))+1); // faccio -1 per ricondurmi agli elementi con cui posso interagire
}


TSP::TSP(string input_file_TSP, string input_file_cities, std::function<double(vector<double>& , const vector<double>& )> input_distance) : op(), distance(input_distance){
	int n_dim=2; // numero di cordinate di ogni città
	bool save=false;
	vector<double> posit(n_dim); // posizione
	vector<vector<double>> V_pos;// vettore in cui salvo le posizioni 
	ifstream input;
	double property;
	input.open(input_file_cities);// file da cui prendo le coordinate
	if (input.is_open()){
		while ( input >> property ){
			// una volta che inizio a immagazzinare questo dato non viene salvato perchè corrisponde all'indice di V_pos+1	
						// cerr << property << endl<< endl;	
			if( int(property) == 1 ) {
				// cerr << property << endl;
				save=true; // da questo momento inizio a immagazzinare i dati
			}
			if(save){
				for(int j=0; j<n_dim; j++){
					input >> property;
					posit[j]=property;
					// cerr << property << endl;
				}
		// cerr << endl;
				V_pos.push_back(posit);
			}
		}
	}else{
		cerr << "ERRORE! impossibile aprire il file di input\n";
	}

	input.close();
	string prop;
	input.open(input_file_TSP);// file da cui prendo le coordinate
	while ( !input.eof() ){
		input >> prop;
		if( prop == "P_m" ){
			input >> P_m;
			if(P_m < 0. or P_m>1.){
				cerr << "PROBLEM: probabilities have to be in the domain: [ 0, 1 ]" << endl;
			}
		} else if( prop == "P_c" ){
			input >> P_c;
			if(P_c < 0. or P_c>1.){
				cerr << "PROBLEM: probabilities have to be in the domain: [ 0, 1 ]" << endl;
			}
		}else if( prop == "POPULATION" ){// individui per popolazione
			input >> property;
			n_individuals=int(property);
			if(n_individuals <= 0){
				cerr << "PROBLEM: population must have a positive numbers of individuals" << endl;
			}
		}else if( prop == "GENERATIONS" ){// individui per popolazione
			input >> property;
			n_generations=int(property);
			if(n_generations <= 0){
				cerr << "PROBLEM: number of generations must be positive" << endl;
			}
		} else if( prop == "ENDINPUT" ){
			break;
		} else{
			cerr << "PROBLEM: unknown input" << endl;
		} 
	}


	int number_cities=V_pos.size();
	V_individuals=vector<individual>(n_individuals);

	// cerr << V_individuals.size() << endl;
	M_distances=vector<vector<double>>(number_cities, vector<double>(number_cities, 0)); // inizializzati i valori a 0
	
	for(int i=0; i<number_cities; i++) {// righe
		for(int j=0; j<i; j++){
			M_distances[i][j]=distance(V_pos[i],V_pos[j]);
			M_distances[j][i]=M_distances[i][j];
		}// sfrutto che la matrice è diagonale per calcolare solo gli elementi sopra alla diagonale (nella diagonale è 0)
	}

	for(int i=0;i<n_individuals;i++){
		V_individuals[i]=individual(number_cities); // uso il costruttore della classe: 'individual'
	}

	int randomizations_steps=10*number_cities; // numero di swaps fatti in ogni individuo per ottenere la configurazione iniziale
	int i1;
	int i2;
	for(int j=0; j<n_individuals; j++){
		for(int i=0; i<randomizations_steps; i++){
			i1=(op.rnd).discr(1,number_cities-1);
			do{
				i2=(op.rnd).discr(1,number_cities-1);
			}while (i1==i2); // gli indici non possono essere uguali
			
			op.swap(V_individuals[j].cities, i1, i2);
			
		}
	}
	V_sons=V_individuals; // fatto per dare la dimensione giusta a V_sons
	sorted=false;


	// for(int j=0; j<n_individuals; j++){
	// 	if(!V_individuals[j].check_f()){
	// 		cerr << "ERRORE!\n";
	// 	} else{
	// 		std::cout << "V_" << j << ": ";
	// 		for (const auto& elem : V_individuals[j].cities) {
	// 			std::cout << elem << " ";
	// 		}
	// 		std::cout << std::endl;
	// 	}
		
	// }
	
}

void TSP::sorting(){
	for(int i=0; i<n_individuals; i++){
		V_individuals[i].loss_function=0.;
		for(int j=0; j<(V_individuals[0].n_cities-1); j++){
			V_individuals[i].loss_function+=M_distances[V_individuals[i].cities[j] - 1][V_individuals[i].cities[j+1] - 1]; // devo mettere il -1 perchè le città partono da 1
		}
		V_individuals[i].loss_function+=M_distances[V_individuals[i].cities[0] - 1][V_individuals[i].cities[V_individuals[i].n_cities-1] - 1];// distanza per tornare al punto di partenza
	}
	std::sort(V_individuals.begin(), V_individuals.end(), [](individual a, individual b) {
        	return a.loss_function < b.loss_function; // Ordine crescente
    	});// usata una lambda function
	sorted=true;
}

void TSP::new_generation(){
	if(!sorted){
		// sorting();
	}

	for(int i=0; i<n_individuals; i++){
		V_sons[i]=op.select(V_individuals);
	}

	for(int j=0; (j+1)<n_individuals; j+=2){ // j è l'indice del primo dei due individui che fanno crossing (faccio crossing tra individui vicini nel vettore figlio)
		if((op.rnd).loaded_coin(P_c)){
			op.cross(V_sons[j].cities, V_sons[j+1].cities, (op.rnd).discr(1, V_individuals[0].n_cities-2));
		}
	}

	int i1;
	int i2;
	int i3; 
	int ind_min;
	int ind_max;
	int max_m;// m massima assumibile (in block_swap)
	for(int i=0; i<n_individuals; i++){ // i è l'indice dell'individuo da mutare
		// if((op.rnd).loaded_coin(P_m)){
		// 	// estraggo a sorte che mutazione fare (ogni mutazione associato un numero intero nell'ordine in cui sono dichiarate)
		// 	int n_mut=(op.rnd).discr(1,4);
		// 	if(n_mut==1){ // swap
		// 			// cerr << n_mut;
		// 		i1=(op.rnd).discr(1,V_individuals[0].n_cities-1);
		// 		i2=(op.rnd).discr(1,V_individuals[0].n_cities-2);// secondo indice iniziale
		// 		if(i2>=i1) i2++; // mi garantisco di non scegliere lo stesso indice di prima
		// 		op.swap(V_sons[i].cities, i1, i2);
		// 	} else if(n_mut==2){// shift
		// 			// cerr << n_mut;
		// 		i1=(op.rnd).discr(1,V_individuals[0].n_cities-2); // indice iniziale traslazione
		// 		i2=(op.rnd).discr(1,V_individuals[0].n_cities-1 - i1); // dimensione blocco traslato
		// 		i3=(op.rnd).discr(1,V_individuals[0].n_cities-i1-i2); // numero di cui tralso
		// 		op.shift(V_sons[i].cities, i1, i2, i3);
		// 	} else if(n_mut==3){ // block_swap
		// 			// cerr << n_mut;
		// 		i1=(op.rnd).discr(1,V_individuals[0].n_cities-1);// primo indice iniziale
		// 		i3=(op.rnd).discr(1,V_individuals[0].n_cities-2);// secondo indice iniziale
		// 		if(i3>=i1){ // mi garantisco di non scegliere lo stesso indice di prima
		// 			//sono in questo if se i3 effettivo è > i1
		// 			i3++;
		// 			ind_min=i1;
		// 			ind_max=i3;
		// 		}else{
		// 			ind_min=i3;
		// 			ind_max=i1;
		// 		} 
		// 		max_m=min(ind_max-ind_min , ind_min+ V_individuals[0].n_cities-1 -ind_max);
		// 		i2=(op.rnd).discr(1,max_m); // lunghezza blocco
		// 		op.block_swap(V_sons[i].cities, i1, i2, i3);
		// 	} else if(n_mut==4){
		// 			// cerr << n_mut;
		// 		i1=(op.rnd).discr(1,V_individuals[0].n_cities-1);
		// 		i2=(op.rnd).discr(2,V_individuals[0].n_cities-1);
		// 		op.inversion(V_sons[i].cities, i1, i2);
		// 	} else cerr << "ERRORE NELLA SCELTA DELLA MUTAZIONE\n";
		
		
		for(int n_mut=1; n_mut<=4; n_mut++){
			// tento di fare ogni mutazione
			
			if((op.rnd).loaded_coin(P_m)){
				if(n_mut==1){ // swap
						// cerr << n_mut;
					i1=(op.rnd).discr(1,V_individuals[0].n_cities-1);
					i2=(op.rnd).discr(1,V_individuals[0].n_cities-2);// secondo indice iniziale
					if(i2>=i1) i2++; // mi garantisco di non scegliere lo stesso indice di prima
					op.swap(V_sons[i].cities, i1, i2);
				} else if(n_mut==2){// shift
						// cerr << n_mut;
					i1=(op.rnd).discr(1,V_individuals[0].n_cities-2); // indice iniziale traslazione
					i2=(op.rnd).discr(1,V_individuals[0].n_cities-1 - i1); // dimensione blocco traslato
					i3=(op.rnd).discr(1,V_individuals[0].n_cities-i1-i2); // numero di cui tralso
					op.shift(V_sons[i].cities, i1, i2, i3);
				} else if(n_mut==3){ // block_swap
						// cerr << n_mut;
					i1=(op.rnd).discr(1,V_individuals[0].n_cities-1);// primo indice iniziale
					i3=(op.rnd).discr(1,V_individuals[0].n_cities-2);// secondo indice iniziale
					if(i3>=i1){ // mi garantisco di non scegliere lo stesso indice di prima
						//sono in questo if se i3 effettivo è > i1
						i3++;
						ind_min=i1;
						ind_max=i3;
					}else{
						ind_min=i3;
						ind_max=i1;
					} 
					max_m=min(ind_max-ind_min , ind_min+ V_individuals[0].n_cities-1 -ind_max);
					i2=(op.rnd).discr(1,max_m); // lunghezza blocco
					op.block_swap(V_sons[i].cities, i1, i2, i3);
				} else if(n_mut==4){
						// cerr << n_mut;
					i1=(op.rnd).discr(1,V_individuals[0].n_cities-1);
					i2=(op.rnd).discr(2,V_individuals[0].n_cities-1);
					op.inversion(V_sons[i].cities, i1, i2);
				} else cerr << "ERRORE NELLA SCELTA DELLA MUTAZIONE\n";
			}

		}
	}
	V_individuals=V_sons;
	sorted=false;
}

int TSP::Get_n_generations(){
	return n_generations;
}

int TSP::Get_n_individuals(){
	return n_individuals;
}

