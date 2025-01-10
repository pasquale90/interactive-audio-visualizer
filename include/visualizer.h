#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <functional>
#include <memory>

#include "fft.h"
#include "roi.h"

#include "videotracker.h"
#include "camera.h"
#include "trigger.h"
class Waveform;

class Visualizer{
public:
    
    /*! @brief Default constructor */
    Visualizer();

    /*! @brief Class destructor */
    ~Visualizer();

    void broadcast();

    void setAudiolizerUpdater(std::function<void(const bool, const bool, const RegionOfInterest&, int&)>);
    void setupShareables(std::shared_ptr<Waveform>);

private:
    const Config &cfg = Config::getInstance();
    Camera camera;
    VideoTracker videoTracker;
    Trigger trigger;
    std::shared_ptr<Waveform> waveform;
    // Spectrogram sp;
    // float *dft;

    cv::Mat visualFrame,cameraFrame;
    cv::Mat camBinaryMask;

    float transpose_ratio_x, transpose_ratio_y;
    int R,G,B;
    int LR,TB;
    int numPointsPerimeter;

//working on them
    void _setToCamera(float);
    void _show_timer(float);
    bool trackingToggle;
    void updateTrackingMode(bool);
    // method to update audioLizer
    std::function<void(const bool, const bool, const RegionOfInterest&, int&)> updateAudioLizer ;

    
// later ..
    void _set_BG_manually(int,bool);
    void _set_FG_manually(const RegionOfInterest&);
    void _create_camMask();    
    bool _showFrame();

    // void midPointCircleDraw(int x_centre, int y_centre, int r);
    // int update_spectrogram();
    void draWaveform();
    void drawSmallcircle(const RegionOfInterest &);
};


#endif