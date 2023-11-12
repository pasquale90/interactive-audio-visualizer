#ifndef RAW_H
#define RAW_H

#include "config.h"
#include <iostream>
#include <math.h>
#include <cstring>

class Waveform{
public:
    Waveform();
    ~Waveform();    
    void set_config(const Config&);

    void prepare_waveform(float*,float*);
    float* getWaveform(float&, float&, int&);
    int getWaveLen();
    
private:
    float *wave;
    int waveLen;
    int buffer_size; 
    int sample_counter;
    
};

#endif