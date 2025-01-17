#include "spectrogram.h"
#include <cmath>
#include <cstddef>

#include <climits>

Spectrogram::Spectrogram(): readpos(0), writepos(0), fft_in(700){
  
  calculateNFFT();
  
  initialize_hamming(numAudioSamples);

  auto fifoSize {numAudioSamples*2};
  ringBuffer.reserve(fifoSize);
  ringBuffer.resize(fifoSize);

  // fft_in = static_cast<fftw_complex*> (fftw_malloc(sizeof(fftw_complex) * numAudioSamples));
  fft_in.reserve(numAudioSamples);
  fft_in.resize(numAudioSamples);
  fft_out = static_cast<fftw_complex*> (fftw_malloc(sizeof(fftw_complex) * numFFTPoints));

  plan = fftw_plan_dft_r2c_1d(numAudioSamples, fft_in.data(), fft_out, FFTW_ESTIMATE); //FFTW_MEASURE

  minMagnitude = INT_MAX, maxMagnitude = 0.;

}

void Spectrogram::calculateNFFT(){
  int W = cfg.dispconf.dispResW.load();
  int n = 2; 
  // n^round(log_n(x)), where log_n(x) = log(x) / log(n)
  // numFFTPoints = std::pow(n, std::round( std::log(W) / std::log(n) )) ;
  numAudioSamples = static_cast<int>(std::pow(n, std::floor( std::log(W) / std::log(n) ))) ; // the closest power of two to a width (lower than width)
  numFFTPoints = numAudioSamples / 2 + 1;
}

void Spectrogram::initialize_hamming(int n){
  hamming_window.reserve(n);
  hamming_window.resize(n);
  for (int i=0;i<n;++i)
    hamming_window[i]=0.54f-0.46f* static_cast<float>(cos(2*PI*i/(n-1)));
}

Spectrogram::~Spectrogram(){
  if (plan) fftw_destroy_plan(plan);
  fftw_cleanup();
  // fftw_free(fft_in); 
  fftw_free(fft_out);
}

int Spectrogram::get_numAudioSamples(){
  return numAudioSamples;
}

int Spectrogram::get_numFFTPoints(){
    return numFFTPoints;
}

bool Spectrogram::write(const float& arg){ // T&& arg --> rvalue reference
    auto writePos = writepos.load(); //??
    auto nextWritePos = (writePos + 1) % ringBuffer.size();

    // if the buffer is full, overwrite the oldest data
    if (nextWritePos == readpos.load()) {
        auto readPos = (readpos.load() + 1) % ringBuffer.size();
        readpos.store(readPos, std::memory_order_release);
    }

    // write data to the buffer
    ringBuffer[writePos] = arg;
    writepos.store(nextWritePos);
    return true;

}

bool Spectrogram::readBatch(std::vector<float>& result, float &min_magnitude, float &max_magnitude) {
    
    // Calculate the start index for reading the last N samples in a forward manner
    size_t startIndex = (writepos.load() - numAudioSamples + 1 + ringBuffer.size()) % ringBuffer.size();

    // Read the N samples starting from startIndex
    size_t readPos = startIndex;
    for (size_t i = 0; i < (size_t)numAudioSamples; ++i) {
        
        // fft_in[i][0]=ringBuffer[readPos] * hamming_window[i];
        // fft_in[i][1]=0;
        fft_in[i]=ringBuffer[readPos] * hamming_window[i];
        
        readPos = (readPos + 1) % ringBuffer.size();  // Move to the next sample (circular)
    }

    fftw_execute(plan);
    
    for (int i = 0; i < numFFTPoints; ++i) {
        // Magnitude of the complex number
        float magnitude = static_cast<float>(std::sqrt(fft_out[i][0] * fft_out[i][0] + fft_out[i][1] * fft_out[i][1]));

        minMagnitude = std::min(minMagnitude,magnitude);
        maxMagnitude = std::max(maxMagnitude,magnitude);

        result[i] = magnitude;
    }

    min_magnitude = minMagnitude;
    max_magnitude = maxMagnitude;
    
    // Update readpos to point to the next sample to be read
    readpos.store((readPos + 1) % ringBuffer.size());

    return true;
}
