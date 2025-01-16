#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <iostream>
#include <vector>

#include "config.h"

#define PI 3.14159

class Spectrogram{
public:

    Spectrogram();
    ~Spectrogram();

    int get_numAudioSamples();
    int get_numFFTPoints();
    
    bool write(const float&);

    bool readBatch(std::vector<float>&, float&, float&);
    
    Spectrogram (const Spectrogram&) = delete;
    Spectrogram (Spectrogram&&) = delete;
    Spectrogram& operator=(const Spectrogram&) = delete;
    Spectrogram& operator=(Spectrogram&&) = delete;
    
private:
    Config &cfg = Config::getInstance();
    
    int numAudioSamples, numFFTPoints;
    std::vector<float> ringBuffer,
        hamming_window;

    std::atomic<size_t> readpos;
    std::atomic<size_t> writepos;

    // std::vector<double> fft_in;
    // double *fft_in;
    std::vector<double> fft_in;
    fftw_complex *fft_out;
    fftw_plan plan;
    
    float minMagnitude,maxMagnitude;

    void calculateNFFT();
    void initialize_hamming(int);


};

#endif