#include <iostream>
#include <fstream>
#include <vector>
#include "finanza.h"
#include "random.h"

using namespace std;



int main (int argc, char *argv[]){
	// ofstream out_errS("./out_err_S");
    // ofstream out_S("./out_S");
	ofstream out_errC("./../out_err_C");
    ofstream out_C("./../out_C_100");
	ofstream out_errP("./../out_err_P");
    ofstream out_P("./../out_P");

	Random rn;
	double r=0.1;
	double sigma=0.25;
	double K=100; // prezzo garantito dal diritto
	double T=1;
	unsigned int M=10000;
	unsigned int N=100; // numero di blocchi
	unsigned int bloc_estr=M/N; //estrazioni per ogni blocco
	double S0=100;
	double S;
	unsigned int steps=1;
	// vector<double> S_T(N,0);
	vector<double> C(N,0);
	vector<double> P(N,0); 
	// vector<double> error_progS(N);
    // vector<double> sum2_progS(N,0);
    // vector<double> sum_progS(N,0);
	vector<double> error_progC(N,0);
    vector<double> sum2_progC(N,0);
    vector<double> sum_progC(N,0);
	vector<double> error_progP(N,0);
    vector<double> sum2_progP(N,0);
    vector<double> sum_progP(N,0);
	
	for( unsigned int i=0;i<N;i++){
		for(unsigned int j=0;j<bloc_estr;j++){
			S=GBM_(rn,steps,S0,sigma,r,T);
			// S_T[i]+=S;
			C[i]+=exp(-r*T)*max(0.,S-K);
			P[i]+=exp(-r*T)*max(0.,K-S);
			// cout << C[i] << "\t" << P[i] << endl;
		}
		// S_T[i]/=bloc_estr;
		C[i]/=bloc_estr;
		P[i]/=bloc_estr;
	}

	for (unsigned int i=0;i<N;i++){ 
		for (unsigned int j=0;j<=i;j++){
			// sum_progS[i]+=S_T[j];
			// sum2_progS[i]+=(S_T[j]*S_T[j]);
			sum_progC[i]+=C[j];
			sum2_progC[i]+=(C[j]*C[j]);
			sum_progP[i]+=P[j];
			sum2_progP[i]+=(P[j]*P[j]);
		}
		// cout << C[i] << "\t" << P[i] << "\t" << i << "\t" << endl;
		// cout << sum_progC[i] << endl; 
		// sum_progS[i]/=(i+1); // cosi diventano medie progressive
		// sum2_progS[i]/=(i+1);
		sum_progC[i]/=(i+1); // cosi diventano medie progressive
		sum2_progC[i]/=(i+1);
		sum_progP[i]/=(i+1); // cosi diventano medie progressive
		sum2_progP[i]/=(i+1);
		// error_progS[i] = error(sum_progS,sum2_progS,i);
		error_progC[i] = error(sum_progC,sum2_progC,i);
		error_progP[i] = error(sum_progP,sum2_progP,i);
		
		out_C << scientific;
		out_errC << scientific;
		out_P << scientific;
		out_errP << scientific;
		
		// out_S << sum_progS[i] << "\t" << i+1 << endl;
		// out_errS << error_progS[i] << "\t" << i+1 << endl;
		out_C << sum_progC[i] << "\t" << i+1 << endl;
		out_errC << error_progC[i] << "\t" << i+1 << endl;
		out_P << sum_progP[i] << "\t" << i+1 << endl;
		out_errP << error_progP[i] << "\t" << i+1 << endl;

	}

	return 0;
}
