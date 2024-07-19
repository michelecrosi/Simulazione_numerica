/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Random__
#define __Random__

#include <vector>

using namespace std;


class cord{
    public:
        cord(const double asc=0,const double ord=0);
        double x;
        double y;
        void operator/=(const double);
		void operator*=(const double);
        double modulo();
};

class Random {

private:
	int m1,m2,m3,m4,l1,l2,l3,l4,n1,n2,n3,n4;

protected:

public:
	// constructors
	Random();
	Random(int riga);
	// destructor
	~Random();
	// methods
	void SetRandom(int * , int, int);
	void SaveSeed();
	double Rannyu(void);
	double Rannyu(double min, double max);
	double Gauss(double mean, double sigma);
	cord vers_2D();
	cord vers_2D_using_pi();
   	bool bool_();
	bool loaded_coin(double P);
	unsigned int discr(const int& N);// numero intero casuale in [1, N]
	int discr(const int& min, const int& max); // numero intero casuale in [min, max]
	vector<vector<int>> couple_perm(const int& N); //permutazioni degli interi da 0 a N-1
};

#endif // __Random__

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
