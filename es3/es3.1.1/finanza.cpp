#include <iostream>
#include <cmath>
#include "random.h"

using namespace std;

double GBM_(Random& rn, const unsigned int& steps, const double& S0, const double& sigma, const double& mu, const double incr){
	// incr è l'incremento temporale
	if(steps==0) return S0;
	// cout << GBM_(rn,steps-1,S0,sigma,mu,incr)*exp((mu-0.5*sigma*sigma)*incr+sigma*rn.Gauss(0,1)*sqrt(incr)) << endl;
	return GBM_(rn,steps-1,S0,sigma,mu,incr)*exp((mu-0.5*sigma*sigma)*incr+sigma*rn.Gauss(0,1)*sqrt(incr));

}