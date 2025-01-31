#include <gtest/gtest.h>
#include "spectrogram.h"
#include "config.h"

TEST(SpectrumFIFOtest, isInitializedProperly) {
    Spectrogram spec;

    int n = spec.get_numAudioSamples();
    bool isPowerOfTwo {(n & (n - 1)) == 0};
    EXPECT_TRUE(isPowerOfTwo); // is a power of two
    
    int fftPoints = spec.get_numFFTPoints();
    EXPECT_EQ(fftPoints, n / 2 + 1); // half the spectrum    
}

TEST(SpectrumFIFOtest, ShortTimeFourierTransformLessPeaks) {
    
    Spectrogram sp;
    Config& cfg = Config::getInstance();
    int N { sp.get_numAudioSamples() }; // numSamples
    int sampleRate = cfg.audconf.sampleRate.load(); 
    float min_magnitude, max_magnitude;
    std::vector<float> fftResult(N, 0.f);

    // Generate a sine wave at multiple frequencies 
    for (int f = 100; f <= 5000; f += 50){
            
        float frequency = static_cast<float>(f);

        // Generate the sine wave
        for (int i = 0; i <= N; i++){
            float sample = sin(2 * M_PI * frequency * i / sampleRate);
            sp.write(sample);
        }

        // Perform the FFT
        sp.readBatch(fftResult, min_magnitude, max_magnitude);

        // Find the exact bin 1
        // int targetBin = round(frequency * N / sampleRate);
        // // Check the bins around the target frequency bin (targetBin ± 1)
        // for (int i = std::max(0, targetBin - 1); i <= std::min(N - 1, targetBin + 1); i++) {
        //     if (fftResult[i] > fftResult[targetBin]) {
        //         targetBin = i;
        //         break;
        //     }
        // }

        // Find the exact bin 2
        float div = static_cast<float>(frequency * N) / static_cast<float>(sampleRate);
        int fl = static_cast<int>( floor(div) );
        int ce = static_cast<int>( ceil(div) );
        int targetBin = fftResult[fl] > fftResult[ce]  ? fl : ce;

        float maxMagnitude = *std::max_element(fftResult.begin(), fftResult.end());
   
        EXPECT_EQ(fftResult[targetBin], maxMagnitude);
    }
}

TEST(SpectrumFIFOtest, CICDTest_TestsIfCIfails) {
    EXPECT_TRUE(false);
}