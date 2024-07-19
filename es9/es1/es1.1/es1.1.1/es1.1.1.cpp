/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include "random.h"

using namespace std;
 
int main (int argc, char *argv[]){
   ofstream out_err("./../out_err");
   ofstream out_av("./../out_av");
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

   int M=100000;
   int N=100;
   int L=M/N; //tiri per ogni blocco
   double sum;
   // double sum2;
   double* av=new double[N]; // media su ogni blocco
   //double* av2=new double[L];
   double* error_prog=new double[N];
   double* sum_prog=new double[N]{};
   double* sum2_prog=new double[N]{};
   double r;
   


   for(int i=0;i<N;i++){
      sum=0;
      // sum2=0;
      for(int k=0;k<L;k++){
         r=rnd.Rannyu();
         sum +=r;
         //sum2 +=r*r;
         //av2[i]=av[i]*av[i];
      }
      av[i]=sum/L;      
   }

   for (int i=0;i<N;i++){  
      for (int j=0;j<=i;j++){
         sum_prog[i]+=av[j];
         sum2_prog[i]+=(av[j]*av[j]);
      }
      sum_prog[i]/=(i+1); // cosi diventano medie progressive
      sum2_prog[i]/=(i+1);
      error_prog[i] = error(sum_prog,sum2_prog,i);
      out_av << sum_prog[i] << "\t" << i+1 << endl;
      out_err << error_prog[i] << "\t" << i+1 << endl;

      //cout << error_prog[i];
   }
   
   return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
