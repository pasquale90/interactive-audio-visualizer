#ifndef CAMERA_H
#define CAMERA_H

#include <atomic>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/hal/interface.h>

#include "config.h"
#include "unused_defines.h"

struct CameraConfig;

class Camera{
public:
    
    /*! @brief Default constructor.*/
    Camera();

    ~Camera();

    /*! @brief Method for capturing frames received from the camera. It is called by Audiolizer*/
    bool capture(cv::Mat&);

    /*! @brief Method that make use of std::atomic variable frameToggle to indicate whether a new frame elapsed.*/
    bool frame_elapsed();

    /*! @brief Method for returning the captured camera frame.*/
    void get_current_frame(cv::Mat&);

    /*! @brief Method for returning the value of actual frames per second, based on hardware capacity*/
    int get_actual_fps();

    Camera (const Camera&) = delete;
    Camera (Camera&&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;
    
private:

    CameraConfig &cameracfg;
    // int camW,camH,fps;

    std::atomic<bool> frameToggle;
    int toggleFrame;
    bool atomicChange;

    cv::VideoCapture cap;
    cv::Mat frame;

    void initialize_camera();
};





#endif