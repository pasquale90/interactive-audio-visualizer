#ifndef VISUALIZER_H
#define VISUALIZER_H

// #include "audio_visualizer.h"
#include <opencv2/opencv.hpp> //OpenCV header to use VideoCapture class//
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <queue>

class Visualizer{
public:
    Visualizer(int,int,int,int);
    Visualizer();
    ~Visualizer();
    int stream_frames(float* in); //
    // static void* visualize(void*);


private:
    int W,H;
    cv::Mat videoframe;
    int R,G,B;

    int update_BG_frame();
    int update_wave_frame();
    int update_spectrogram(float*);

    float *dft;
    float *buffer;
    std::queue<float> wave;
    int x_trans;
    int redxtrans; //reduces the time that x transition changes (used for the waveform)
    bool ascX;
    
    int SR;
    int buffer_size;

    int incrR,incrG,incrB;
    bool ascR,ascG,ascB;
    int MIN{0},MAX{200};
    void change_BG_color();
    int update_counter;
    int update_ratio;

};



#endif