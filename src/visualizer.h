#ifndef VISUALIZER_H
#define VISUALIZER_H

// #include "audio_visualizer.h"
#include <opencv2/opencv.hpp> //OpenCV header to use VideoCapture class//
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include "opencv2/highgui/highgui.hpp"

class Visualizer{
public:
    Visualizer(int width,int height);
    Visualizer();
    ~Visualizer();
    int stream_frames(float*);
private:
    cv::Mat videoframe;
    int R,G,B;

    int empty_visualizer();
    // AudioVisualizer av;

    int incrR,incrG,incrB;
    bool ascR,ascG,ascB;
    int MIN{0},MAX{255};
    void change_colors();
    int update_counter;
    int update_ratio=4;

};



#endif