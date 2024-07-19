#include <iostream>
#include <cmath>
#include <fstream>
#include "random.h"
#include "Buffon.h"

using namespace std;


const double pi=3.14159265358979323846;


// ago::ago(){
//     L=1;
//     pos.x=0;
//     pos.y=0;
//     ang=0;
// }

ago::ago(cord posizione, const double angolo, const double lung){
    pos=posizione;
    L=lung;
    ang=angolo; // questo in realtà non lo uso per l'esperimento
}
ago::ago(const double x, const double y, const double angolo, const double lung){
    pos.x=x;
    pos.y=y;
    ang=angolo; 
    L=lung;
}
void ago::in_ang(double angolo){
    ang=angolo;
}
void ago::in_pos(cord posizione){
    pos=posizione;
}
void ago::in_pos(double asc, double ord){
    pos.x=asc;
    pos.y=ord;
}
void ago::in_L(double lunghezza){
    L=lunghezza;
}
double ago::get_ang(){
    return ang;
}
cord ago::get_pos(){
    return pos;
}
double ago::get_L(){
    return L;
}
ago::~ago(){}

Buffon::Buffon(const double dis, const double lun, const cord minim, const cord maxim){
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ; //prendo due numeri dal file: Primes
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rn.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;
    rn.SaveSeed();

    a.in_L(lun);
    d=dis;
    min=minim;
    max=maxim;
    if(d<lun){
        cerr << "ATTENZIONE! è stato impostata una lunghezza dell'ago pari a L=" << lun << " mentre la distanza tra le righe è di d=" << dis << ", si consiglia di usare d<L (anche se non d<<L)" << endl;
    }
    if((d/a.get_L())<0.01){
        cerr << "ATTENZIONE! è stato impostata una lunghezza dell'ago pari a L=" << lun << " mentre la distanza tra le righe è di d=" << dis << ", si consiglia di usare d<L invece di d<<L" << endl;
    }
}

// non capisco perchè così non funziona

// Buffon::Buffon(){
//     double dis=1;
//     double lun=0.8;
//     cord minim=cord(-100,-100);
//     cord maxim=cord(100,100);
//     int seed[4];
//     int p1, p2;
//     ifstream Primes("Primes");
//     if (Primes.is_open()){
//         Primes >> p1 >> p2 ; //prendo due numeri dal file: Primes
//     } else cerr << "PROBLEM: Unable to open Primes" << endl;
//     Primes.close();

//     ifstream input("seed.in");
//     string property;
//     if (input.is_open()){
//         while ( !input.eof() ){
//             input >> property;
//             if( property == "RANDOMSEED" ){
//             input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
//             rn.SetRandom(seed,p1,p2);
//             }
//         }
//         input.close();
//     } else cerr << "PROBLEM: Unable to open seed.in" << endl;
//     rn.SaveSeed();

//     a.in_L(lun);
//     d=dis;
//     min=minim;
//     max=maxim;
//     // cout << d << a.get_L();
//     if(d<lun){
//         cerr << "ATTENZIONE! è stato impostata una lunghezza dell'ago pari a L=" << lun << " mentre la distanza tra le righe è di d=" << dis << ", si consiglia di usare d>L (anche se non d>>L)" << endl;
//     }
//     if((d/a.get_L())<0.01){
//         cerr << "ATTENZIONE! è stato impostata una lunghezza dell'ago pari a L=" << lun << " mentre la distanza tra le righe è di d=" << dis << ", si consiglia di usare d>L invece di d>>L" << endl;
//     }
// }
double Buffon::get_d(){
    return d;
}
double Buffon::get_L(){
    return a.get_L();
}
cord Buffon::get_min(){
    return min;
}
cord Buffon::get_max(){
    return max;
}
ago Buffon::get_a(){
    return a;
}
bool Buffon::thr(){ // ho true se ho una intersezione
    a=ago(rn.Rannyu(min.x,max.x),rn.Rannyu(min.y,max.y));
    // return floor((a.get_pos()).y/d)!=floor(((a.get_pos()).y+a.get_L()*sin(a.get_ang()))/d); 
    return floor((a.get_pos()).y/d)!=floor(((a.get_pos()).y+a.get_L()*(rn.vers_2D()).y)/d); 
    
    // le righe orizzontali partono da y=0
    // ho usato la funzione floor di cmath e non un cast a intero perchè il cast a intero non mi vedrebbe le intersezioni con y=0
    // (floor tronca sempre verso il basso mentre il cast toglie i decimali)
}

