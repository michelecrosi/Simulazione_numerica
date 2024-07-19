#ifndef __INTEGRALS_HPP__
#define __INTEGRALS_HPP__

#include <iostream>
#include <cmath>
#include "random.h"

using namespace std;

class Integral{
	public:
		Integral();
		Integral(const double&, const double&);
		virtual ~Integral();
		virtual double Integrate(const unsigned int&, const FunzioneBase&)=0;
		double GetNSteps() const;
		double GetPasso() const;
		double GetIntegral() const;
		double GetA() const;
		double GetB() const;
	protected:
		void CheckInterval(const double&, const double&);//serve per integrare da una  x maggiore a una minore
		int sign; //serve per integrare da una  x maggiore a una minore
		unsigned int NSteps;
		double a, b, m_Integral, h;
		double max(const double&, const double&) const;
		double min(const double&, const double&) const; 
};

class Midpoint: public Integral{
	public:
		Midpoint();
		Midpoint(const double&, const double&);
		virtual double Integrate(const unsigned int&, const FunzioneBase&) override;
};

class Simpson: public Integral{
	public:
		Simpson(const double&, const double&);
		virtual double Integrate(const unsigned int&, const FunzioneBase&) override;
};

class Trapezi: public Integral{
	public:
		Trapezi(const double&, const double&);
		virtual double Integrate(const unsigned int&, const FunzioneBase&) override;
		double Integrate_prec(const double&, const FunzioneBase&);
		double GetIncertezza() const;
	protected:
		double Incertezza;
};

class IntegMedia: public Integral{
	public:
		IntegMedia();
		IntegMedia(const double&, const double&);
		virtual double Integrate(const unsigned int&, const FunzioneBase&) override;
	protected:
		Random Gen; //necessario metterlo nei membri altrimenti se ne creo uno nuovo con lo stesso seme ogni volta ottengo sempre lo stesso risultato anche se lo calcolo diverse volte mentre gira il programma
};

class IntegHM: public Integral{
	public:
		IntegHM();
		IntegHM(const double&, const double&, const double&);
		void SetFmax(const double&);
		double GetFmax() const;
		unsigned int GetNtot() const;
		unsigned int GetNhit() const;
		virtual double Integrate(const unsigned int&, const FunzioneBase&) override;
	protected:
		unsigned int Ntot;
		unsigned int Nhit;
		RandomGen Gen;
		double Fmax;

};

#endif