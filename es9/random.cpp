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
#include <cmath>
#include <cstdlib>
#include "random.h"

using namespace std;

cord::cord(const double asc,const double ord){
   x=asc;
   y=ord;
}
void cord::operator/=(const double cost){
   x/=cost;
   y/=cost;
}
void cord::operator*=(const double cost){
   x*=cost;
   y*=cost;
}
double cord::modulo(){
   return sqrt((x*x)+(y*y));
}


Random :: Random(){
   int seed[4];
   int p1, p2;
   ifstream Primes("./INPUT/Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ; //prendo due numeri dal file: Primes
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("./INPUT/seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
         input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
         SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
   SaveSeed();
}

Random :: ~Random(){}

void Random :: SaveSeed(){
   ofstream WriteSeed;
   WriteSeed.open("./OUTPUT/seed.out");
   if (WriteSeed.is_open()){
      WriteSeed << l1 << " " << l2 << " " << l3 << " " << l4 << endl;;
   } else cerr << "PROBLEM: Unable to open random.out" << endl;
  WriteSeed.close();
  return;
}

double Random :: Gauss(double mean, double sigma) {
   double s=Rannyu();
   double t=Rannyu();
   double x=sqrt(-2.*log(1.-s))*cos(2.*M_PI*t);
   return mean + x * sigma;
}

double Random :: Rannyu(double min, double max){
   return min+(max-min)*Rannyu();
}

double Random :: Rannyu(void){
  const double twom12=0.000244140625;
  int i1,i2,i3,i4;
  double r;

  i1 = l1*m4 + l2*m3 + l3*m2 + l4*m1 + n1;
  i2 = l2*m4 + l3*m3 + l4*m2 + n2;
  i3 = l3*m4 + l4*m3 + n3;
  i4 = l4*m4 + n4;
  l4 = i4%4096;
  i3 = i3 + i4/4096;
  l3 = i3%4096;
  i2 = i2 + i3/4096;
  l2 = i2%4096;
  l1 = (i1 + i2/4096)%4096;
  r=twom12*(l1+twom12*(l2+twom12*(l3+twom12*(l4))));

  return r;
}

void Random :: SetRandom(int * s, int p1, int p2){
  m1 = 502;
  m2 = 1521;
  m3 = 4071;
  m4 = 2107;
  l1 = s[0];
  l2 = s[1];
  l3 = s[2];
  l4 = s[3];
  n1 = 0;
  n2 = 0;
  n3 = p1;
  n4 = p2;

  return;
}

cord Random::vers_2D(){
   double x_p=1;// valori iniziali impostati in modo che entri nel while
   double y_p=1;
   while((x_p*x_p)+(y_p*y_p)>1){
      x_p=Rannyu(-1,1);
      y_p=Rannyu(-1,1);
   }
   cord ver(x_p,y_p);
   // cout << ver.x << " " << ver.y << endl << ver.modulo() << endl;
   ver/=ver.modulo();
   // cout << ver.x << " " << ver.y << endl << endl;
   return ver;
}

cord Random::vers_2D_using_pi(){
   double theta=Rannyu(0.,2.*M_PI);
   double x_p=cos(theta);
   double y_p=sin(theta);
   cord ver(x_p,y_p);

   return ver;
}

bool Random::bool_(){
   if(discr(2)==1) return true;
   else return false;
}

bool Random :: loaded_coin(double P){// gli do la probabilità che mi ritorni true
   if(P>1) cerr << "WARNING! A PROBABILITY MORE THAN 1 WAS INSERTED IN THE FUNCTION: 'loaded_coin(double)'\n";
   if(Rannyu() <= P) return true;
   else return false;
}

unsigned int Random::discr(const int& N){ // generato un numero casuale fra 1 e N
   if(N==0) cerr << "ERRORE! Il metodo \"discr\" accetta solo interi strettamente maggiori di 0\n";
   return (floor(Rannyu(0,N))+1);
}

int Random::discr(const int& min, const int& max){ // generato un numero casuale intero fra min e max
   if(min>max){
      cerr << "ERRORE! nel metodo: \"discr\" è stato inserito un valore massimo minore del valore minimo\n";
   } 
   return floor(Rannyu(min,max+1));
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
