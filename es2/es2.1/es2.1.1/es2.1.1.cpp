#include <iostream>
#include <fstream>
#include <vector>
#include "funzioni.hpp"
#include "random.h"

using namespace std;
 
int main (int argc, char *argv[]){
    ofstream out_err("./../out_err_unif");
    ofstream out_I("./../out_I_unif");
    out_err << scientific;
    out_I << scientific;
    int N=100; //numero blocchi 
    double M=10000000; //nunero di tiri totali
    int bloc_estr=M/N; //estrazioni per ogni blocco
    // double sum2;
    vector<double> I(bloc_estr);
    vector<double> error_prog(N);
    vector<double> sum2_prog(N);
    vector<double> sum_prog(N);
    Random Gen;
    Cos c;
    for(int i=0;i<N;i++){
        for (int k=0;k<bloc_estr;k++){
            I[i]+=c.f(Gen.Rannyu());
            // I[i]+=(pi/2.)*cos(pi*Gen.Rannyu()/2.);
            
	    }
        I[i]/=bloc_estr;
    }

    for (int i=0;i<N;i++){  
        for (int j=0;j<=i;j++){
            sum_prog[i]+=I[j];
            sum2_prog[i]+=(I[j]*I[j]);
        }
        sum_prog[i]/=(i+1); // così diventano medie progressive
        sum2_prog[i]/=(i+1);
        error_prog[i] = error(sum_prog,sum2_prog,i);
        out_I << sum_prog[i] << "\t" << i+1 << endl;
        out_err << error_prog[i] << "\t" << i+1 << endl;

        //cout << error_prog[i];
    }
	return 0;
}
