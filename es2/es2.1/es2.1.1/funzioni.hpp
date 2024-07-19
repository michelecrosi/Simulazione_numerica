#ifndef __FUNZIONI_HPP__
#define __FUNZIONI_HPP__

#include <iostream>
#include <cmath>

struct coordinata{
    double x;
    double y;
};

class FunzioneBase{
    public:
		FunzioneBase(){};
		virtual double f(const double&) const =0;
		virtual ~FunzioneBase(){};//è buona prassi definire il distruttore di una classe madre astratta come virtual in modo che il distruttore della classe figlia venga invocato correttamente quando un oggetto di tipo classe figlia viene distrutto
};

class Parabola: public FunzioneBase{
    public:
        Parabola();
        Parabola(const double&, const double&, const double&);
        void UpdateDelta();
        void UpdateVertex();
        virtual ~Parabola();
        virtual double f(const double&) const override; //override e` un comando che impone al compilatore di controllare che i parametri qui presenti siano dello stesso tipo di quelli nella classe astratta
        void Seta(const double&);
        void Setb(const double&);
        void Setc(const double&);
        double Geta() const;
        double Getb() const;
        double Getc() const;
        double GetVertexX() const;
        double GetVertexY() const;
    protected:
        double a,b,c;
        double Delta;
        coordinata Vertex;
};

class Non_analitic: public FunzioneBase{
	virtual double f(const double&) const override;
};

class Sin: public FunzioneBase{
    virtual double f(const double&) const override;
};

class Cos: public FunzioneBase{
    public:
        virtual double f(const double&) const override;
};


class Gaussian: public FunzioneBase{
    public:
        Gaussian(const double&, const double&);
        virtual double f(const double&) const override;
        void SetU(const double&);
        void SetSigma(const double&);
        double GetU() const;
        double GetSigma() const;
    private:
        double u; //centro della Gaussiana
        double sigma;
};

#endif