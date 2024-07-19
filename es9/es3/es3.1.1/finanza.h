
#ifndef __finanza__
#define __finanza__

#include "random.h"

// class GMB{
// 	public:
// 		double mu;
// 		double sigma;
// 		Random rn;
// 		GMB();
// 		GMB(const double);
// 		const unsigned int& max, const double& S0, const double& sigma, const double& mu, const double incr
// }


double GBM_(Random& rn, const unsigned int& max, const double& S0, const double& sigma, const double& mu, const double incr);

// template <typename T>
// double max(T A, T B){
//    if (A<B){
//       return B;
//    }
//    else{
//       return A;
//    }    
// }

#endif