#ifndef SIGNAL_H
#define SIGNAL_H

#include "config.h"
#include <math.h>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif


class Signal{
public:
    Signal();
    Signal(int, int);
    ~Signal();
    void set_config(const Config&);

    void mix(double*,double*,double*,int);
    void prepareSine(int);
    double getSineL();
    double getSineR();
private:
    int sampleRate, bufferSize;
    int prevfreq;
    double *sine;
    int phaseL;
    int phaseR;
    double phase;
    double rads_per_sample;
    double amplitude;
};

#endif