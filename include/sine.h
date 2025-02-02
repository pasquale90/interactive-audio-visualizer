#ifndef SINE_H
#define SINE_H

#include <functional>
class Config;
struct AudioConfig;
struct Tone;

/*!
 * @brief Class responsible for generating sine wave signals for audio processing.
*/
class Sine{

public:

    /*! @brief Default constructor */
    Sine();

    /*!
     * @brief Function to set the visualizer update callback.
     * @param callback - The function to be called when a new visualization update is required.
     * @return void
    */
    void setVisualizerUpdater(std::function<void(float)>);

    /*!
     * @brief Function to generate a mono sine wave signal for a given tone.
     * @param tone - The tone for which the sine wave signal should be generated.
     * @param buffer - The buffer to store the generated sine wave signal.
     * @note The buffer size received has two channels (to allow compatibillity with stereo), but the the first channel is only used for writting.
    */
    void setMonoSignal(Tone&, float*[2]);
    
    /*!
     * @brief Function to generate a stereo sine wave signal for a given tone.
     * @param tone - The tone for which the stereo sine wave signal should be generated.
     * @param buffer - The buffer to store the generated stereo sine wave signal.
    */
    void setStereoSignal(Tone&, float*[2]);

private:
    const AudioConfig& audiocfg;
    int prevfreq;
    float phase;
    float rads_per_sample;

    std::function<void(float)> updateVisualizer;
};

#endif