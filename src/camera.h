#ifndef CAMERA_H
#define CAMERA_H

#include "config.h"
#include <atomic>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/hal/interface.h>


class Camera{
public:
    // Camera(int,int,int);
    Camera(const Config&);
    // Camera(const Camera& camera);
    Camera();
    ~Camera();

    void setConfig(const Config&);
    
    bool capture(cv::Mat&);
    bool _frame_elapsed();
    void display_config();

    // void init_ROI(cv::Mat&,int,int,int,int);
    void get_current_frame(cv::Mat&);
    int get_fps();
    int get_actual_fps();
private:
    std::atomic<bool> frameToggle;
    int toggleFrame;
    bool atomicChange;

    int camW,camH,fps;
    double proper_fps;
    cv::VideoCapture cap;
    
    int lastframe;
    cv::Mat frame;

    void initialize_camera();
};





#endif