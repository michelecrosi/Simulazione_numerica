#include "integrals.hpp"


Integral::Integral(){

}
Integral::Integral(const double& a_, const double& b_){
	CheckInterval(a_,b_);
}
Integral::~Integral(){
	
}
void Integral::CheckInterval(const double& a_, const double& b_){
	a=min(a_,b_);
	b=max(a_,b_);
	if(a_>b_)
		sign=-1;
	if(a_==b_)
		sign=0; //l'integrale in un punto e` nullo e il suo segno lo metto a zero
	if(a_<b_)
		sign=1;
}


double Integral::max(const double& p, const double& q) const{
	if(p<=q)
		return q;
	else 
		return p;
}
double Integral::min(const double& p, const double& q) const{
	if(p>=q)
		return q;
	else
		return p;
}
double Integral::GetNSteps() const{
	return NSteps;
}
double Integral::GetPasso() const{
	return h;
}
double Integral::GetIntegral() const{
	return m_Integral;
}
double Integral::GetA() const{
	return a;
}
double Integral::GetB() const{
	return b;
}

Midpoint::Midpoint(){

}
Midpoint::Midpoint(const double& a_, const double& b_):Integral(a_,b_){
	
}
double Midpoint::Integrate(const unsigned int& steps, const FunzioneBase& funct){
	if(steps==0){
		cout << "ERROR! the number of steps can't be negative or zero!\n"; 
		exit(-62);
	}
	if(a==b){
		m_Integral=0;
	}else{
		NSteps=steps;
		m_Integral=0;
		h=(b-a)/NSteps;
		for (unsigned int i=0;i<NSteps;i++){
			m_Integral+=funct.f(a+(i+0.5)*h);  
		}
		m_Integral*=sign*h; //moltiplicato per h alla fine cosi` non rischio di avere numeri troppo piccoli che mi vengono troncati
	}
	return m_Integral;
}

Simpson::Simpson(const double& a, const double& b):Integral(a,b){

}
double Simpson::Integrate(const unsigned int& Ns, const FunzioneBase& g){
	if(Ns==0){
		cout << "ERROR! the number of steps can't be negative or zero!\n"; 
		exit(-84);
	}
	if(a==b){
		m_Integral=0;
	}else{
		if(Ns%2!=0){
			cout << "Il numero di passi è stato aumentato di uno, in quanto l'algoritmo di Simpson prevede un numero pari di passi\n";
			NSteps=Ns+1;
		}
		else
			NSteps=Ns;
		double h=(b-a)/NSteps;
		double somma=g.f(a);
		for(int i=1;i<=NSteps-1;i++){
			if(i%2==1){
				somma+=4*g.f(a+(i*h));
			}
			else
				somma+=2*g.f(a+(i*h));
		}
		somma+=g.f(b);
		m_Integral=somma*sign*h/3.;
	}
	return m_Integral;
}

Trapezi::Trapezi(const double& a, const double& b):Integral(a,b){

}

double Trapezi::GetIncertezza() const{
	return Incertezza;
}

double Trapezi::Integrate(const unsigned int& Ns, const FunzioneBase& g){
	if(Ns==0){
		cout << "ERROR! the number of steps can't be negative or zero!\n"; 
		exit(-121);
	}if(a==b){
		m_Integral=0;
	}else{
		double h_=b-a;
		double D=2*h_;
		m_Integral=(g.f(a)+g.f(b))*0.5;
		for(int i=1;i<=Ns;i++){
			D=h_;
			h_*=0.5;	
			for(int j=0;j<pow(2,i-1);j++){// 2^(i-1) è il numero di punti nuovi, rispetto al passo precedente, da calcolare
				m_Integral+=g.f(a+h_+j*D);
			}
		}
		m_Integral*=sign*(b-a)/(pow(2.,Ns));
	}
	return m_Integral;
}

double Trapezi::Integrate_prec(const double& prec, const FunzioneBase& g){
	if(prec<=0){
		cout << "ERROR! the precision can't be negative or zero!\n"; 
		exit(-143);
	}
	double I2n;
	if(a==b){
		m_Integral=0;
		I2n=0;
		Incertezza=0;
	}else{
		Incertezza=prec;
		double In, i;
		i=2; //i è il numero degli step usati nell'algoritmo
		I2n=Integrate(2,g);
		In=Integrate(1,g);
		double inc=4*abs(I2n-In)/3.;
		while((4*abs(I2n-In)/3.)>=prec){
			i++;
			In=I2n;
			I2n=Integrate(i,g);
			inc=4*abs(I2n-In)/3.;
		}
		Incertezza=inc;
	} 
	return I2n; //anche se ho stimato l'errore di In mi conviene ritornare I2n poichè tanto l'ho già calcolato e sicuramente ha un errore minore alla stima fatta
}

IntegMedia::IntegMedia():
	Gen()
{

}
IntegMedia::IntegMedia(const double& a_, const double& b_):
	Integral(a_,b_),
	Gen()
{
	
}

double IntegMedia::Integrate(const unsigned int& steps, const FunzioneBase& funct){
	if(steps==0){
		cout << "ERROR! the number of steps can't be negative or zero!\n"; 
		exit(-27);
	}
	double x;
	NSteps=steps;
	m_Integral=0;
	for (unsigned int i=0;i<NSteps;i++){
		x=Gen.Rannyu(a,b);
		m_Integral+=funct.f(x);
	}
	m_Integral=(m_Integral*(b-a))/NSteps;

	return m_Integral;
}// io uso questa


IntegHM::IntegHM():
	Gen(1)
{
	Nhit=0;
	Ntot=0;
}
IntegHM::IntegHM(const double& a_, const double& b_, const double& max):
	Integral(a_,b_),
	Gen(1)
{
	Nhit=0;
	Ntot=0;
	Fmax=max;
}

void IntegHM::SetFmax(const double& max){
	Fmax=max;
}

double IntegHM::GetFmax() const{
	return Fmax;
}

unsigned int IntegHM::GetNtot() const{
	return Ntot;
}

unsigned int IntegHM::GetNhit() const{
	return Nhit;
}

double IntegHM::Integrate(const unsigned int& steps, const FunzioneBase& funct){
	if(steps==0){
		cout << "ERROR! the number of steps can't be negative or zero!\n"; 
		exit(-27);
	}
	Nhit=0;
	Ntot=0;//devo inizializzarli a 0 perchè potrei volere calcolare più integrali o più volte lo stesso integrale
	double x,y;
	NSteps=steps;
	for (unsigned int i=0;i<NSteps;i++){
		x=Gen.Unif(a, b);
		y=Gen.Unif(0, Fmax);
		Ntot++;
		if(y<funct.f(x)){
			Nhit++;
		}
	}
	m_Integral=(b-a)*Fmax*Nhit/Ntot;

	return m_Integral;
}