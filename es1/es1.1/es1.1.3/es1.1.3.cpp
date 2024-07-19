#include <iostream>
#include <fstream>
#include <string>
#include "random.h"

using namespace std;
 
int main (int argc, char *argv[]){

   ofstream out_chi2("./../out_chi2");
   
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

/*
   int incr=10000; // aumento di n all'aumentare di j nella consegna
   int n_incr=100; // numero degli incrementi (ossia di j possibili nella consegna)
   int M=100; //(numero di intervalli)
   double I=1./M; //larghezza intervalli
   double r;
   double E;  // numero di estrazioni aspettate in ogni intervallo
   int n_=10000;
   int in; // la uso per immagazzinare il numero dell'intervallo corrispondente a una estrazione
   // double sum2;
   double* n=new double[M]{}; // vettore degli n_i (ossia numero di eventi in ogni sottointervallo)
   double* chi2=new double[n_incr]{}; // vettore dei chi^2_j
   for(int i=0;i<n_incr;i++){
      E=n_/double(M);
      for(int k=0;k<n_;k++){
         r=rnd.Rannyu();
         in=int(r/I);
         // cerr << r << endl << I << endl << in << endl;
         n[in]++;
      }
      for(int k=0;k<M;k++){
         chi2[i]+=((n[k]-E)*(n[k]-E))/E;
      }
      out_chi2 << chi2[i] << "\t" << i+1 << endl;
      n_+=incr;
   }
*/

   int n_=10000;
   int num_j=100; // numero degli incrementi (ossia di j possibili nella consegna)
   int M=100; //(numero di intervalli)
   double I=1./M; //larghezza intervalli
   double r;
   double E=double(n_)/double(M);  // numero di estrazioni aspettate in ogni intervallo
   int in; // la uso per immagazzinare il numero dell'intervallo corrispondente a una estrazione
   // double sum2;
   double* n=new double[M]{}; // vettore degli n_i (ossia numero di eventi in ogni sottointervallo)
   double* chi2=new double[num_j]{}; // vettore dei chi^2_j
   for(int i=0;i<num_j;i++){
      for(int k=0;k<n_;k++){
         r=rnd.Rannyu();
         in=int(r/I); // indice corrispondente al sottointervallo corrispondente al numero estratto
         // cerr << r << endl << I << endl << in << endl;
         n[in]++;
      }
      for(int k=0;k<M;k++){
         chi2[i]+=((n[k]-E)*(n[k]-E))/E;
         n[k]=0; //resetto il contatore a 0 in tutti i sottointervalli
      }
      out_chi2 << chi2[i] << "\t" << i+1 << endl;
   }
   
   
   return 0;
}