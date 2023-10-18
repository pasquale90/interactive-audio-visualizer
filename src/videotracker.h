// gets the frame from camera and applies tracking to the frame 
// --> gives back certain information (center, width, speed) that are going to be transfomed into music data 
#ifndef TRACKING_H
#define TRACKING_H

#include <iostream>
#include <vector>
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/tracking/tracking.hpp>

#include "config.h"
#include "camera.h"

class VideoTracker{
public:

    VideoTracker();
    ~VideoTracker();

    void setConfig(const Config& cfg);
    void display_config();
    void _capture();
    bool get_frame_elapsed();

    bool update(std::pair<int,int>&, cv::Mat&);
private:
    
    Camera camera;

    void _cropROI(bool);                         // change these names
    void _updateROI();                           // change these names

    cv::Ptr<cv::Tracker> tracker;
    std::pair<int,int> boxCenter;
    std::pair<int,int> currboxCenter;

    cv::Rect centerBox;
    cv::Rect2d boundingBox;
    int LTRBwhCxCy[8]; // roi coordinates LeftTopRightBottom
    int thickness=1;
    int H,W,fps;
    
    int ROIoffset; // SIZE OF THE BOX
    int ROIw8sec;
    cv::Mat prevROI,ROI;
    cv::Mat currFrame;
    cv::Mat BG;
    bool patternlocked;

    int framecounter;

    bool _w84userInteraction();
    void _initialize_tracker();

    int similarity_threshold=150; //TEMPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
    bool _check_similarity();
    void _init_timer(); 
};

#endif