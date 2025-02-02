#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <functional>
#include <vector>
#include "camera.h"
#include "videotracker.h"
#include "trigger.h"
#include "waveform.h"
#include "spectrogram.h"
class RegionOfInterest;
class Tone;

/*!
 * @brief This class is responsible for managing the camera feed, tracking objects, triggering, and broadcasting the visualized frame to the IAV pipeline.
*/
class Visualizer{
public:
    
    /*! @brief Default constructor */
    Visualizer();

    /*! @brief Class destructor */
    ~Visualizer();

    /*!
     * @brief Copy constructor is deleted to prevent accidental use.
    */
    Visualizer(const Visualizer&) = delete;

    /*!
     * @brief Move constructor is deleted to prevent accidental use.
    */
    Visualizer(Visualizer&&) = delete;

    /*!
     * @brief Copy assignment operator is deleted to prevent accidental use.
    */
    Visualizer& operator=(const Visualizer&) = delete;

    /*!
     * @brief Move assignment operator is deleted to prevent accidental use.
    */
    Visualizer& operator=(Visualizer&&) = delete;

    /*!
     * @brief Broadcasts the visualized frame to the IAV processing pipeline.
     * @note Runs in a separate thread.
     * @return void
    */
    void broadcast();

    /*!
     * @brief Sets the function that is to be used for receiving the Audiolizer's update signal.
     * @param std::function<void(const bool, const bool, const RegionOfInterest&, Tone&)> - The function to be called for receiving the Audiolizer's update signal.
     * @note AudiolizerUpdater @param1 const bool trackingUpdated - variable that indicates whether there is a new tracking signal..
     * @note AudiolizerUpdater @param2 const bool trackingEnabled - variable that indicates whether the tracking is enabled or not.
     * @note AudiolizerUpdater @param3 const RegionOfInterest& roi - variable passed by reference that updates the value of the current tracking signal.
     * @note AudiolizerUpdater @param4 Tone& - the tone object for storing the current frequency and volume.
     * @return void
    */
    void setAudiolizerUpdater(std::function<void(const bool, const bool, const RegionOfInterest&, Tone&)>);

    /*!
     * @brief Receives the audio signal by the Sine instance, to update the Waveform and Spectrogram instances.
     * @param float - the audio signal received by the Sine instance.
     * @return void
    */
    void updateAudioSignal(float);

private:

    const Config &cfg = Config::getInstance();

    Camera camera;
    VideoTracker videoTracker;
    Trigger trigger;
    Waveform waveform;
    Spectrogram spectrogram;
    std::vector<float> specMagnitude;

    cv::Mat visualFrame,cameraFrame;
    cv::Mat camBinaryMask;

    float transpose_ratio_x, transpose_ratio_y;
    int R,G,B;
    int LR,TB;
    int numPointsPerimeter;
    int leftMidFreq, rightMidFreq;

    void _setToCamera(float);
    void _show_timer(float);

    bool trackingToggle;
    void updateTrackingMode(bool);
    std::function<void(const bool, const bool, const RegionOfInterest&, Tone&)> updateAudioLizer ;
    
    void _set_BG_manually(Tone&);
    void _set_FG_manually(const RegionOfInterest&);
    void _create_camMask();    
    bool _showFrame();
    void _set_freq_midBoundaries();

    void draWaveform();
    void drawSpectrogram();
    void drawSmallcircle(const RegionOfInterest &);
};


#endif