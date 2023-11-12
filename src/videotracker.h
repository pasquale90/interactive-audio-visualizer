// gets the frame from camera and applies tracking to the frame 
// --> gives back certain information (center, width, speed) that are going to be transfomed into music data 
#ifndef TRACKING_H
#define TRACKING_H

#include <iostream>
#include <utility>
#include <opencv2/video/tracking.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/tracking/tracker.hpp>

#include "camera.h"
#include "roi.cpp"

class VideoTracker{
public:

    VideoTracker();
    ~VideoTracker();

    void setConfig(const Config& cfg);
    void display_config();
    void capture();
    bool update(RegionOfInterest&, cv::Mat&);
    bool pattern_locked();
    bool tickTock();
private:

    bool _tracking_updated();
    void _initialize_tracker();
    bool _check_similarity();
    void _init_timer(); 
    void _show_timer();

    std::atomic<bool> trackingToggle;
    int toggleTrack;
    bool atomicChange;

    Camera camera;
    int H,W,fps;
    int radius;
    int ROIw8sec;
    int framecounter;
    cv::Ptr<cv::Tracker> tracker;
    RegionOfInterest boxCenter;
    cv::Mat BG,ROI,currFrame;
    cv::Rect centerBox;
    cv::Rect2d boundingBox;
    
    std::atomic<int> currboxCenter_x,currboxCenter_y,currboxCenter_w,currboxCenter_h;
    std::atomic<bool> patternlocked;
    
    int similarity_threshold=150; //regression value . Use an heuristic method to calculate it.
};

#endif