#include <iostream>
#include <fstream>
#include <string>
#include "random.h"

using namespace std;
 
int main (int argc, char *argv[]){
   ofstream out_err_var("./../out_err_var");
   ofstream out_var("./../out_var");
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
   int L=M/N; //tiro per ogni blocco
   double sum;
   // double sum2;
   double* var=new double[L];
   double* error_prog=new double[N];
   double* sum_prog=new double[N]{};
   double* sum2_prog=new double[N]{};
   double r;
   


   for(int i=0;i<N;i++){
      sum=0;
      // sum2=0;
      for(int k=0;k<L;k++){
         r=rnd.Rannyu();
         sum +=((r-0.5)*(r-0.5));
         //sum2 +=r*r;
      }
      var[i]=sum/L;      
   }

   for (int i=0;i<N;i++){  
      for (int j=0;j<=i;j++){
         sum_prog[i]+=var[j];
         sum2_prog[i]+=(var[j]*var[j]);
      }
      sum_prog[i]/=(i+1); // cosi diventano medie progressive
      sum2_prog[i]/=(i+1);
      error_prog[i] = error(sum_prog,sum2_prog,i);
      out_var << sum_prog[i] << "\t" << i+1 << endl;
      out_err_var << error_prog[i] << "\t" << i+1 << endl;

      //cout << error_prog[i];
   }
   
   return 0;
}