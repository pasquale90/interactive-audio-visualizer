#ifndef CAMERA_H
#define CAMERA_H

#include "config.h"
#include <atomic>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/hal/interface.h>


class Camera{
public:
    
    /*! @brief Default constructor.*/
    Camera();

    /*! @brief Class destructor.*/
    ~Camera();

    /*! @brief Class implicit constructor*/
    void setConfig(const Config&);

    /*! @brief Method for capturing frames received from the camera. It is called by Audiolizer*/
    bool capture(cv::Mat&);

    /*! @brief Method that make use of std::atomic variable frameToggle to indicate whether a new frame elapsed.*/
    bool frame_elapsed();

    /*! @brief displays config variables*/
    void display_config();

    /*! @brief Method for returning the captured camera frame.*/
    void get_current_frame(cv::Mat&);

    /*! @brief Method for returning the value of frames per second*/
    int get_fps();

    /*! @brief Method for returning the value of actual frames per second, based on hardware capacity*/
    int get_actual_fps();
private:
    std::atomic<bool> frameToggle;
    int toggleFrame;
    bool atomicChange;

    int camW,camH,fps;
    double proper_fps;
    cv::VideoCapture cap;
    
    cv::Mat frame;

    void initialize_camera();
};





#endif