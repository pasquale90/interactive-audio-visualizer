#include "gui/opencvfps.h"

double getCVfps_approx(const char* cameraDevice){
    cv::VideoCapture video(cameraDevice);
    
    // Check camera
    // if (!video.isOpened()) {
    //     return -1;
    // }

    return video.get(cv::CAP_PROP_FPS);
}