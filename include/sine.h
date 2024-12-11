#ifndef SINE_H
#define SINE_H

class Config;
struct AudioConfig;

class Sine{

public:

    /*! @brief Default constructor */
    Sine();

    void getMonoSignal(int, float*[2]);
    void getStereoSignal(int, float*[2]);

private:
    const AudioConfig& audiocfg;
    int prevfreq;
    float phase;
    float rads_per_sample;
    float amplitude;
};

#endif