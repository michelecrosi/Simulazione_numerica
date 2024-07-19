
#ifndef __Random__
#define __Random__

#include <cmath>
#include <vector>
#include "funzioni.hpp"


using namespace std;

// This class contains functions for generating random numbers using the RANNYU algorithm

class cord{
    public:
        cord(const double asc=0,const double ord=0);
        double x;
        double y;
        void operator/=(const double);
        double modulo();
};

// class vers{
//    public:
//       vers(const cord punta=cord(1,0));
//       vers(const double x_punta=0, const double y_punta=0);

//       cord p; // punta del versore
// };

class Random {

   private:
      int m1,m2,m3,m4,l1,l2,l3,l4,n1,n2,n3,n4;

   protected:

   public:
      // Default constructor
      Random();

      // Destructor
      ~Random();

      // Method to set the seed for the RNG (random number generator)
      void SetRandom(int * , int, int);

      // Method to save the seed to a file
      void SaveSeed();

      // Method to generate a random number in the range [0,1)
      double Rannyu(void);

      // Method to generate a random number in the range [min,max)
      double Rannyu(double min, double max);

      // Method to generate a random number with a Gaussian distribution
      double Gauss(double mean, double sigma);

      double exp(double l);

      double lor(double g, double u);

      cord vers_2D();

      double lineare(const lin& l,double min, double max); // [min, max) è l'intervallo dell'ascisse della distribuzione
};

double error(double* AV,double* AV2, int n);

template <typename T>
double error(vector <T> AV, vector <T> AV2, int n){
   if (n==0){
      return 0;
   }
   else{
      return sqrt((AV2[n] - (AV[n]*AV[n]))/n);
   }    
} 

#endif
