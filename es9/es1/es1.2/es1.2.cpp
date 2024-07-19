#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "random.h"

using namespace std;
 
int main (int argc, char *argv[]){

    
    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ; //prendo due numeri dal file: Primes
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;



    // for(int i=0; i<20; i++){
    //    cout << rnd.Rannyu() << endl;
    // }

    rnd.SaveSeed();


    int numeri_N=4;
    int numero_S=10000; // numero di medie che calcolo
    vector <vector <double>> S_uni(numeri_N, vector<double>(numero_S,0));
    vector <vector <double>> S_exp(numeri_N, vector<double>(numero_S,0));
    vector <vector <double>> S_lor(numeri_N, vector<double>(numero_S,0));
    // il primo indice mi dice quanti dati uso per calcolare le medie il secondo mi dice quale media considero
    vector<int> N = {1, 2, 10, 100};
    vector <ofstream> N_uni(4);
    vector <ofstream> N_exp(4);
    vector <ofstream> N_lor(4);

    vector <string> nomeDirectory={"uniforme", "esponenziale", "lorenziana"} ;

    // Verifica se la directory esiste già
    for(int i=0;i<nomeDirectory.size();i++){
        if (!filesystem::exists(nomeDirectory[i])) {
        // Creo una directory
            if(filesystem::create_directory(nomeDirectory[i])) {
                cout << "La directory: " << nomeDirectory[i] <<" è stata creata con successo." << endl;
            } else {
                cerr << "Errore durante la creazione della directory: " << nomeDirectory[i] << endl;
                return 1;
            }
        }
    }
    
    for(int i=0;i<numeri_N;i++){
        N_uni[i].open("./uniforme/out_N"+to_string(i+1));
        N_exp[i].open("./esponenziale/out_N"+to_string(i+1));
        N_lor[i].open("./lorenziana/out_N"+to_string(i+1));

    }

    for(int i=0;i<numeri_N;i++){
        for(int k=0;k<numero_S;k++){
            for(int j=0;j<N[i];j++){
                S_uni[i][k]+=rnd.Rannyu();
                S_exp[i][k]+=rnd.exp(1);
                S_lor[i][k]+=rnd.lor(1,0);
            }
            S_uni[i][k]/=N[i];
            S_exp[i][k]/=N[i];
            S_lor[i][k]/=N[i];

            N_uni[i] << S_uni[i][k] << endl;
            N_exp[i] << S_exp[i][k] << endl;
            N_lor[i] << S_lor[i][k] << endl;
        }
    }
   return 0;
}