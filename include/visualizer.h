#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "raw.h"
#include "fft.h"
#include "roi.h"
#include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include "opencv2/highgui/highgui.hpp"
// #include <opencv2/core/mat.hpp>

class Visualizer{
public:
    
    /*! @brief Default constructor */
    Visualizer();

    /*! @brief Class implicit destructor */
    void shutdown();

    /*! @brief Class implicit constructor */
    void setup(const Config&);

    /*! @brief Method resplonsible for visualization.
     * @param[in] float* left audio buffer filled with a sine wave tone
     * @param[in] float* right audio buffer filled with a sine wave tone
     * @param[in] cv::Mat - frame obtained from camera
     * @param[in] bool frameElapsed - whether a new frame elapsed from the camera
     * @param[in] bool trackEnabled - whether a new update emerged from tracking
     * @param[in] int - frequency tone - currently affects the coloring (future releases will probably leave such features to the audio buffer signal analysis)
     * @param[in] RegionOfInterest - tracking updates 
     */
    int and_Sound_into_Image(float*, float*, cv::Mat, bool, bool, int, RegionOfInterest);
    
private:
    Waveform wf;
    int *wave;
    // Spectrogram sp;
    // double *dft;
    cv::Mat visualFrame;
    cv::Mat camBinaryMask;

    int W,H,fps,SR,buffer_size;
    int R,G,B;
    int fmin, fmax;
    int LR,TB;
    int numPointsPerimeter;
    // int bufferCount;
    // int buffersPerFrame;

    void _set_BG_manually(int,bool);
    void _set_FG_manually(cv::Mat, RegionOfInterest);
    void _setToCamera(cv::Mat);
    void _create_camMask(int,int);    
    bool _showFrame();

    // void midPointCircleDraw(int x_centre, int y_centre, int r);
    // int update_spectrogram();
    void draWaveform(int,int);
    void drawSmallcircle(int,int,int,int,int,int);
};



#endif