#ifndef SINE_H
#define SINE_H

#include <functional>
class Config;
struct AudioConfig;
struct Tone;

class Sine{

public:

    /*! @brief Default constructor */
    Sine();

    void setVisualizerUpdater(std::function<void(float)>);

    void setMonoSignal(Tone&, float*[2]);
    void setStereoSignal(Tone&, float*[2]);

private:
    const AudioConfig& audiocfg;
    int prevfreq;
    float phase;
    float rads_per_sample;

    std::function<void(float)> updateVisualizer;
};

#endif