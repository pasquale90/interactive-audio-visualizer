#ifndef VISUALIZER_H
#define VISUALIZER_H

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

    int empty_visualizer();
};



#endif