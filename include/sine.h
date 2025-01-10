#ifndef SINE_H
#define SINE_H

#include <memory>

#include "waveform.h"
class Config;
struct AudioConfig;
class Waveform;

class Sine{

public:

    /*! @brief Default constructor */
    Sine();

    void setupShareables(const std::shared_ptr<Waveform>&);

    void setMonoSignal(int, float*[2]);
    void setStereoSignal(int, float*[2]);

private:
    const AudioConfig& audiocfg;
    int prevfreq;
    float phase;
    float rads_per_sample;
    float amplitude;

    std::shared_ptr<Waveform> waveform;

};

#endif