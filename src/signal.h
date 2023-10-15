#ifndef SIGNAL_H
#define SIGNAL_H

#include <math.h>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif


class Signal{
public:
    Signal(int bufferSize);
    ~Signal();
    // void fillBuffer(double*,double*);
    void prepareSine(int);
    double getSineL();
    double getSineR();
private:
    int prevfreq;
    double *sine;
    int phaseL;
    int phaseR;
    int maxFrequency=700;
};

#endif