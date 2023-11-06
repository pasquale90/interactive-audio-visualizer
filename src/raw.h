#ifndef RAW_H
#define RAW_H



#include "config.h"
#include <iostream>
#include <math.h>
#include <cstring>
/// temporary
#include <opencv2/opencv.hpp> //OpenCV header to use VideoCapture class//
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include "opencv2/highgui/highgui.hpp"
///

class Waveform{
public:
    Waveform();
    Waveform(int,int,int);
    ~Waveform();    

    // void set_config(int,int,int);
    void set_config(const Config&);

    void prepare_waveform(float*,float*);
    float* getWaveform(float&, float&, int&);
    int getWaveLen();

    void getMeanBuffers(double *);
private:
    float *wave;
    int waveLen;
    int buffCount, currIndex;
    int buffer_size; 
    int buffers_per_frame;
    int width;

    void get_ring_xy(double*,int);

    int sample_counter,bpf;
    cv::Mat tempWaveFrame;
    int W,H;
};

#endif