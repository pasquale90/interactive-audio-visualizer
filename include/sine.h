#ifndef SINE_H
#define SINE_H

#include <memory>

#include "waveform.h"
class Config;
struct AudioConfig;
class Waveform;
struct Tone;

class Sine{

public:

    /*! @brief Default constructor */
    Sine();

    void setupShareables(const std::shared_ptr<Waveform>&);

    void setMonoSignal(Tone&, float*[2]);
    void setStereoSignal(Tone&, float*[2]);

    // void setVolume(float);
    
private:
    const AudioConfig& audiocfg;
    int prevfreq;
    float phase;
    float rads_per_sample;
    // float amplitude;

    std::shared_ptr<Waveform> waveform;

};

#endif