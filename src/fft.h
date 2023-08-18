#ifndef FFT_H
#define FFT_H

// #include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <iostream>

#define PI 3.14159

class Spetrogram{
public:
    Spetrogram();
    Spetrogram(int,int);
    ~Spetrogram();
    double* computeFFT(double*,double*);
    
    
private:
    int buffer_size;
    int height;
    double *hamming_window;
    fftw_complex *fft_in;
    fftw_complex *fft_out;
    fftw_plan p;
    double *log_fft_out;
    
    void hamming();

};

#endif