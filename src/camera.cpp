#include <iostream>
#include <opencv2/core/hal/interface.h>
#include "camera.h"
#include "config.h"

Camera::Camera() : cameracfg(Config::getInstance().camconf){
    frameToggle.store(false);
    toggleFrame=false;
    cv::Mat frame(cameracfg.camResH.load(),cameracfg.camResW.load(),CV_8UC3);
    initialize_camera();
}

void Camera::initialize_camera(){

    int device = cameracfg.device.back()-'0';
    int width = cameracfg.camResW.load();
    int height = cameracfg.camResH.load();
    int fps = static_cast<int>(cameracfg.frameRate.load());

    if (!cap.open(device, cv::CAP_V4L2)) {
        std::cerr << "Error: Could not open camera " << device << std::endl;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    int actualWidth = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int actualHeight = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double actualFps = cap.get(cv::CAP_PROP_FPS);

    if (actualWidth != width || actualHeight != height || actualFps != fps) {
        
        std::cerr << "Warning: Camera properties might not be set correctly!" << std::endl;
        
        cameracfg.camResW.store(actualWidth);
        cameracfg.camResH.store(actualHeight);
        cameracfg.frameRate.store(actualFps);
    }
}

Camera::~Camera(){
    frame.release();
    cap.release();
    std::cout<<"Camera object destructed"<<std::endl;
}

bool Camera::frame_elapsed(){
    atomicChange = frameToggle.load();
    if (frameToggle.load()!=toggleFrame){     // process the current input from camera
        toggleFrame=atomicChange;
        return true;
    }else
        return false;
}

bool Camera::capture(cv::Mat& frame){

    cap.read(frame);

    if(!(frame.empty())){
        frameToggle.store(!frameToggle.load());
        return true;
    }
    return false;
}