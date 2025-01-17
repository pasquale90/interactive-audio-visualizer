#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <functional>
#include <memory>
#include <vector>

#include "roi.h"

#include "videotracker.h"
#include "camera.h"
#include "trigger.h"
#include "waveform.h"
#include "spectrogram.h"

class Visualizer{
public:
    
    /*! @brief Default constructor */
    Visualizer();

    /*! @brief Class destructor */
    ~Visualizer();

    Visualizer(const Visualizer&) = delete;
    Visualizer(Visualizer&&) = delete;
    Visualizer& operator=(const Visualizer&) = delete;
    Visualizer& operator=(Visualizer&&) = delete;

    void broadcast();

    void setAudiolizerUpdater(std::function<void(const bool, const bool, const RegionOfInterest&, int&)>);

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


    void _setToCamera(float);
    void _show_timer(float);

    bool trackingToggle;
    void updateTrackingMode(bool);
    std::function<void(const bool, const bool, const RegionOfInterest&, int&)> updateAudioLizer ;
    
    void _set_BG_manually(int);
    void _set_FG_manually(const RegionOfInterest&);
    void _create_camMask();    
    bool _showFrame();

    void draWaveform();
    void drawSpectrogram();
    void drawSmallcircle(const RegionOfInterest &);
};


#endif