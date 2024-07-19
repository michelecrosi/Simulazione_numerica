#ifndef __Buffon__
#define __Buffon__



class ago {

    private:
        cord pos;
        double ang;
        double L;
    public:
        // ago();
        ago(cord, const double angolo=0,const double lung=1);
        ago(const double x=0, const double y=0, const double angolo=0, const double lung=1);
        void in_ang(double);
        void in_pos(cord);
        void in_pos(double,double);
        void in_L(double);
        cord get_pos();
        double get_ang();
        double get_L();
        ~ago();
};

class Buffon{
    private:
        Random rn;
        cord max;
        cord min;
        double d;
        ago a;
    public:
        Buffon(const double dis=1, const double lun=0.8, const cord minim=cord(-100,-100), const cord maxim=cord(100,100)); // le cordinate in questo costruttore sono quelle che delimitano il campo su cui getto l'ago
        // Buffon();
        double get_d();
        double get_L();
        cord get_min();
        cord get_max();
        ago get_a();
        bool thr();
};


#endif