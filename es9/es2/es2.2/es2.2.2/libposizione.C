#include "libposizione.hpp"

posizione::posizione(){
    x=0;
    y=0;
    z=0;
}
posizione::posizione(const double& a,const double& b,const double& c){
    x=a;
    y=b;
    z=c;
}
double posizione::distance(){
    return sqrt((pow(x,2))+(pow(y,2))+(pow(z,2)));
}
double posizione::distance(const posizione& P) const{
    return sqrt(pow(x-P.x,2)+pow(y-P.y,2)+pow(z-P.z,2));
}
double posizione::Getx() const{
    return x;
}
double posizione::Gety() const{
    return y;
}
double posizione::Getz() const{
    return z;
}
void posizione::Setx(const double & value){
    x=value;
}
void posizione::Sety(const double & value){
    y=value;
}
void posizione::Setz(const double & value){
    z=value;
}
double posizione::GetRho_c(){//cordinate cilindriche
    return sqrt((pow(x,2))+(pow(y,2)));
}
double posizione::GetRho(){
    return distance();
}
double posizione::GetPhi(){
    if(y>0)
        return acos(x/GetRho_c());
    if(x==0 && y==0){
        cerr << "ERROR:\n it is impossible to define phi for the vector (0,0,z)";
        return -999;
	}
    if(y<0){
        return (acos(-x/GetRho_c()))+M_PI;
	}
    if(x<0 && y==0)
        return M_PI;
    return 0; //caso con x>0 && y==0
}
double posizione::GetTheta(){
    if(x==0 && y==0 && z==0){
		cerr << "ERROR:\n it is impossible to define theta for the null vector";
		return -999;
	}
    return acos(z/GetRho());
}
void posizione::PrintCartesiane(){
	cout << "(" << Getx() << "," << Gety() << "," << Getz() << ")";
}
void posizione::PrintCilindriche(){
	cout << "(" << GetRho_c() << "," << GetPhi() << "," << Getz() << ")";
}
void posizione::PrintSferiche(){
    cout << "(" << GetRho() << "," << GetPhi() << "," << GetTheta() << ")";
}
double& posizione::operator[](const unsigned int& i){
    // non devo controllare i sia positivo poichè è un unsigned int
    if(i>2) cerr << "ERRORE! è stato inserito come indice della classe \"posizione\" i=" << i << " mentre deve essere inserito un intero compreso tra 0 e 2 (estremi inclusi)";
    if(i==0) return x;
    if(i==1) return y;
    return z;
}
void posizione::operator+=(const posizione& p){
    posizione p_temp=p;
    for(int i=0;i<3;i++){
        (*this)[i]+=p_temp[i];  // this è un puntatore a un membro della classe all'interno di cui scrivo il codice
    }
}


posizione operator*(const posizione& p, const double& c){
    posizione p_temp=p;
    for(int i=0;i<3;i++){
        p_temp[i]*=c;
    }
    return p_temp;
}
posizione operator*(const double& c, const posizione& p){
    posizione p_temp=p;
    for(int i=0;i<3;i++){
        p_temp[i]*=c;
    }
    return p_temp;
}