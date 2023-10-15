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
    Camera(int,int,int);
    Camera(const Config&);
    // Camera(const Camera& camera);
    Camera();
    ~Camera();

    void setConfig(const Config&);
    
    void capture();
    // void temp_capture(); // delete meeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
    bool get_frame();
    void display_config();

private:
    std::atomic<bool> frameToggle;
    // std::atomic<int> frameToggle;
    // static std::atomic<bool> toggle;
    // bool toggle;
    
    // void set_frame(int);


    int camW,camH,fps;
    cv::VideoCapture cap;
    
    // int frameCount;
    // int update_ratio=4;
    int lastframe;
    cv::Mat frame;

    void initialize_camera();
};





#endif