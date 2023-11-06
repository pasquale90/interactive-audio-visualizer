#ifndef RAW_H
#define RAW_H

#include <iostream>
#include <math.h>

class Waveform{
public:
    Waveform(int,int,int);
    ~Waveform();    

    void prepare_waveform(int,double*);
    double* getWaveform(double&, double&);
    int getWaveLen();

    void getMeanBuffers(double *);
private:
    double *wave;
    int waveLen;
    int buffer_size; 
    int buffers_per_frame;
    int width;

    void get_ring_xy(double*,int);

};

#endif