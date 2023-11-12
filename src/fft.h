#ifndef FFT_H
#define FFT_H

// #include <iostream>
#include "config.h"
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <iostream>

#define PI 3.14159

class Spectrogram{
public:
    Spectrogram();
    Spectrogram(int,int,int);
    ~Spectrogram();

    void set_config(const Config&);
    void computeFFT(double*,double&,double&);
    void prepare_spectrogram(double*,double*);
    
private:
    int buffer_size;
    int height;
    int buffers_per_frame;
    double *hamming_window;
    fftw_complex *fft_in;
    fftw_complex *fft_out;
    fftw_plan p;
    
    double *FFTcol;
    int siglen;
    
    void hamming(int);

};

#endif