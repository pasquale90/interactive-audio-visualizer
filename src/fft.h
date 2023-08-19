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
    Spetrogram(int,int,int);
    ~Spetrogram();
    double* computeFFT(double*);
    void prepare_spectrogram(int,double*);
    
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