#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "raw.h"
#include "fft.h"
#include "roi.cpp"
#include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include "opencv2/highgui/highgui.hpp"
// #include <opencv2/core/mat.hpp>

class Visualizer{
public:
    
    Visualizer();
    ~Visualizer();
    void setConfig(const Config&);
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