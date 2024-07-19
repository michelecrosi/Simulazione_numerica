#include <iostream>
#include <fstream>
#include <vector>
#include "random.h"

using namespace std;


 
int main (int argc, char *argv[]){
    ofstream out_RW_discr_err("./../out_RW_discr_err");
    ofstream out_RW_discr("./../out_RW_discr");

    


    int N=100; // numero blocchi 
    int steps=100;
    int M=10000; // nunero random walks
    int bloc_cam=M/N; // cammini per ogni blocco
    double a=1.;
    unsigned int dis; // valore discreto generato randomicamente
    int sgn=0; 
    double mod;
    double sum_mod; // ci metto la somma dei moduli quadri in un blocco
    // double sum2;
    
    vector <vector<int>> vec_pos(M,vector<int>(3,0)); // posso usare interi poichè a=1
    vector<double> error_prog(N);
    vector<double> av(steps,0); // media a step diversi fra tutti i blocchi 
    vector<double> av2(steps,0); // media dei quadrati a step diversi fra tutti i blocchi 
    Random Gen;

    for(int i=0;i<steps;i++){
        for(int j=0;j<N;j++){
            sum_mod=0;
            for (int k=0;k<bloc_cam;k++){
                dis=Gen.discr(3)-1;
                // cout << dis << endl;
                if(Gen.bool_()) sgn=1;
                else sgn=-1;
                vec_pos[k+bloc_cam*j][dis]+=sgn*a;
                mod=modulo(vec_pos[k+bloc_cam*j]);
                sum_mod+=mod*mod;
            }
            av[i]+=sqrt(sum_mod/bloc_cam);
            av2[i]+=sum_mod/bloc_cam;
        }
        av[i]/=N;
        av2[i]/=N;
        // cout << av[i] << "\t" << av2[i]<< endl;
        out_RW_discr << av[i] << "\t" << i+1 << endl;
        out_RW_discr_err << error(av[i],av2[i],N) << "\t" << i+1 << endl; 
    }  

    
	return 0;
}
