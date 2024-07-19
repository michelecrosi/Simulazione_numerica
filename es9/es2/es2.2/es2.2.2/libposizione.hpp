#ifndef __LIBPOSIZIONE__
#define __LIBPOSIZIONE__

#include <iostream>
#include <cmath>

using namespace std;

class posizione{
    public:
		posizione();
		posizione(const double&,const double&,const double&);
		double distance(); //it returns the distance from the origin
		double distance(const posizione&) const; //it returns the distance from another position
		double Getx() const;
		double Gety() const;
		double Getz() const;
		void Setx(const double &);
		void Sety(const double &);
		void Setz(const double &);
		double GetRho();
		double GetPhi();
		double GetTheta();
		double GetRho_c();
		void PrintCartesiane();
		void PrintCilindriche();
		void PrintSferiche();
		double& operator[](const unsigned int&);
		void operator+=(const posizione&);
		

    protected:
		double x;
		double y;
		double z;
};

posizione operator*(const posizione&, const double&);
posizione operator*(const double&, const posizione&);


#endif