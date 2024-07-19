#include "funzioni.hpp"

const double pi=3.14159265358979323846;

Parabola::Parabola(){

}
Parabola::Parabola(const double& A, const double& B, const double& C){
    a=A;
    b=B;
    c=C;
    Delta=pow(B,2)-4*A*C;
    Vertex.x=-B/(2*A);
    Vertex.y=-Delta/(4*A);
}
void Parabola::UpdateDelta(){
    Delta=pow(b,2)-4*a*c;
}
void Parabola::UpdateVertex(){
    Vertex.x=-b/(2*a);
    Vertex.y=-(pow(b,2)-4*a*c)/(4*a);
}
Parabola::~Parabola(){

}
double Parabola::f(const double& x) const{ //non devo mettere ne virtual ne override nella dichiarazione
    return a*pow(x,2)+b*x+c;
}
void Parabola::Seta(const double& A){
    a=A;
}
void Parabola::Setb(const double& B){
    b=B;
}
void Parabola::Setc(const double& C){
    c=C;
}
double Parabola::Geta() const{
    return a;
}
double Parabola::Getb() const{
    return b;
}
double Parabola::Getc() const{
    return c;
}
double Parabola::GetVertexX() const{
    return Vertex.x;
}
double Parabola::GetVertexY() const{
    return Vertex.y;
}

double Non_analitic::f(const double& x) const{
	//return tan(x)-x; //la tangente vicino hai punti in cui esplode (dove cosx=0) vieneinfluenzata molto da piccole variazioni, per questo motivo è necessario moltiplicare per cosx
	return sin(x)-x*cos(x);
}

double Sin::f(const double& x) const{
	//return tan(x)-x; //la tangente vicino hai punti in cui esplode (dove cosx=0) vieneinfluenzata molto da piccole variazioni, per questo motivo è necessario moltiplicare per cosx
	return sin(x);
}

double Cos::f(const double& x) const{
	//return tan(x)-x; //la tangente vicino hai punti in cui esplode (dove cosx=0) vieneinfluenzata molto da piccole variazioni, per questo motivo è necessario moltiplicare per cosx
	return (pi/2.)*cos(pi*x/2.);
}

Gaussian::Gaussian(const double& U, const double& SIGMA){
    u=U;
    sigma=SIGMA;
}
double Gaussian::f(const double& x) const{
	return (exp(-0.5*(pow((x-u)/sigma,2))))/(sigma*sqrt(2*M_PI)); //Gaussiana normalizzata
}
void Gaussian::SetU(const double& U){
    u=U;
}
void Gaussian::SetSigma(const double& SIGMA){
    sigma=SIGMA;
}
double Gaussian::GetU() const{
    return u;
}
double Gaussian::GetSigma() const{
    return sigma;
}