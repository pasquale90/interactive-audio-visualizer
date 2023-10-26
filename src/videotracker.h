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

    bool update(std::pair<int,int>&, cv::Mat&);
    bool _tracking_updated();
    bool _pattern_locked();

    bool tickTock();
private:
    std::atomic<bool> trackingToggle;
    std::atomic<bool> frameTick;
    int toggleTrack;
    bool atomicChange;

    Camera camera;

    cv::Ptr<cv::Tracker> tracker;
    std::pair<int,int> boxCenter;
    // std::pair<int,int> currboxCenter;
    std::atomic<int> currboxCenter_x,currboxCenter_y;

    int thickness=1;
    int H,W,fps;
    
    int radius; // SIZE OF THE BOX
    int ROIw8sec;
    cv::Mat BG,ROI,currFrame;
    cv::Rect centerBox;
    cv::Rect2d boundingBox;
    
    std::atomic<bool> patternlocked;
    int framecounter;
    
    int similarity_threshold=150; //TEMPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
    
    void _initialize_tracker();
    bool _check_similarity();
    void _init_timer(); 
};

#endif