// gets the frame from camera and applies tracking to the frame 
// --> gives back certain information (center, width, speed) that are going to be transfomed into music data 
#ifndef TRACKING_H
#define TRACKING_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/tracking/tracking.hpp>

#include "config.h"
#include "camera.h"

class Tracking{
public:

    Tracking();
    ~Tracking();

    void setConfig(const Config& cfg);
    void display_config();
    void _capture();
    bool get_frame_elapsed();

private:

    Camera camera;

    // int W=640;
    // int H=480;
    // int fps=30;
    int ROIoffset;// = H*0.1; // SIZE OF THE BOX
    // int time2wait = 5; //seconds
    int ROIw8sec;

};

#endif