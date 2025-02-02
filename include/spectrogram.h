#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H


#include <fftw3.h>
#include <vector>
#include "config.h"

#define PI 3.14159

/*!
 * @brief Ring buffer class to generate a spectrogram of the audio signal using the Fast Fourier Transform (FFT).
 * @note The spectrogram is calculated using the FFTW3 library.
*/
class Spectrogram{
public:

    /*!
     * @brief Default constructor
    */
    Spectrogram();

    /*!
     * @brief Class destructor
    */
    ~Spectrogram();
    
    /*!
     * @brief Copy constructor is deleted to prevent accidental use.
    */
    Spectrogram (const Spectrogram&) = delete;
    
    /*!
     * @brief Move constructor is deleted to prevent accidental use.
    */
    Spectrogram (Spectrogram&&) = delete;
    
    /*!
     * @brief Copy assignment operator is deleted to prevent accidental use.
    */
    Spectrogram& operator=(const Spectrogram&) = delete;
    
    /*!
     * @brief Move assignment operator is deleted to prevent accidental use.
    */
    Spectrogram& operator=(Spectrogram&&) = delete;
    
    /**
     * @brief Returns the number of audio samples used to calculate the spectrogram
     * @note the number of audio samples defines the capacity of the ring buffer (FIFO).
     * @return void
     */
    int get_numAudioSamples();
    
    /**
     * @brief Returns the number of FFT points
     * @return int - The number of FFT points
     */
    int get_numFFTPoints();
    
    /**
     * @brief Function used to set the ring buffer with a value to the write position of the FIFO.
     * @param const float& - the audio sample to be written
     * @return bool - Returns true if successful, false otherwise
    */
    bool write(const float&);

    /*!
     * @brief Function used to read a batch of audio samples from the FIFO and store them in a provided vector.
     * @param std::vector<float>& - the vector to store the read audio samples
     * @param float& - the minimum magnitude of the audio samples in the batch
     * @param float& - the maximum magnitude of the audio samples in the batch
     * @return bool - Returns true if successful, false otherwise
     */
    bool readBatch(std::vector<float>&, float&, float&);

private:
    Config &cfg = Config::getInstance();
    
    int numAudioSamples, numFFTPoints;
    std::vector<float> ringBuffer,
        hamming_window;

    std::atomic<size_t> readpos;
    std::atomic<size_t> writepos;

    // fftw_complex *fft_in;
    std::vector<double> fft_in;
    fftw_complex *fft_out;
    fftw_plan plan;
    
    float minMagnitude,maxMagnitude;

    void calculateNFFT();
    void initialize_hamming(int);


};

#endif