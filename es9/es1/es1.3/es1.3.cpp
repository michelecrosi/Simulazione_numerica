#include <iostream>
#include <fstream>
#include <vector>
#include "random.h"
#include "Buffon.h"

using namespace std;
 
int main (int argc, char *argv[]){
    ofstream out_err("out_err");
    ofstream out_av("out_pi");
    Buffon B;
    int N=100; //numero blocchi 
    double N_hit;
    double M=10000000; //nunero di tiri totali
    double N_thr;
    int bloc_thr=M/N; //tiri per ogni blocco
    // double sum2;
    vector<double> pi_exp(bloc_thr);
    vector<double> error_prog(N);
    vector<double> sum_prog(N);
    vector<double> sum2_prog(N);



    for(int i=0;i<N;i++){
        N_hit=0;
        N_thr=0;
        for(int k=0;k<bloc_thr;k++){
            if(B.thr()) N_hit++;
            N_thr++;
        }
        pi_exp[i]=(2*B.get_L()*N_thr)/(N_hit*B.get_d());      
    }

    for (int i=0;i<N;i++){  
        for (int j=0;j<=i;j++){
            sum_prog[i]+=pi_exp[j];
            sum2_prog[i]+=(pi_exp[j]*pi_exp[j]);
        }
        sum_prog[i]/=(i+1); // cosi diventano medie progressive
        sum2_prog[i]/=(i+1);
        error_prog[i] = error(sum_prog,sum2_prog,i);
        out_av << sum_prog[i] << "\t" << i+1 << endl;
        out_err << error_prog[i] << "\t" << i+1 << endl;

        //cout << error_prog[i];
    }

    
}
