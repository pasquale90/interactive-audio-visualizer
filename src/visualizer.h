#ifndef VISUALIZER_H
#define VISUALIZER_H

// #include "audio_visualizer.h"
#include "config.h"
#include "raw.h"
#include "fft.h"
#include <opencv2/opencv.hpp> //OpenCV header to use VideoCapture class//
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <queue>
#include <random>


class Visualizer{
public:
    // Visualizer(int,int,int,int,int);
    Visualizer(const Config&);
    Visualizer();
    ~Visualizer();
    int stream_frames(double* in,bool isBeat); //
    int and_Sound_into_Image(double*in, cv::Mat, int, bool, bool);
    // static void* visualize(void*);
    void setConfig(const Config&);


private:
    Config conf;
    int W,H;
    cv::Mat videoframe;
    bool showFrame();
    int R,G,B;

    int update_BG_frame(); // remove this isDownBeat dependency
    int update_wave_frame();
    int update_spectrogram();

    double *dft;
    double *buffer;

    int x_trans;
    int redxtrans; //reduces the time that x transition changes (used for the waveform)
    bool ascX;

    int f_x_trans;
    
    int fps;
    int SR;
    int buffer_size;

    int incrR,incrG,incrB;
    bool ascR,ascG,ascB;
    int MIN{0},MAX{200};
    void change_BG_color();
    void _change_BG_color(int,bool);
    int bufferCount;
    int buffersPerFrame;
    
    Waveform *wf;
        
    Spectrogram *sp;
    int beatCount;
    
};



#endif