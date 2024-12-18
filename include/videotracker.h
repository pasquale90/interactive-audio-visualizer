// gets the frame from camera and applies tracking to the frame 
// --> gives back certain information (center, width, speed) that are going to be transfomed into music data 
#ifndef TRACKING_H
#define TRACKING_H

#include <iostream>
#include <utility>
#include <opencv2/tracking.hpp>
#include <opencv2/features2d.hpp>

#include "camera.h"
#include "roi.h"
#include "timer.h"

constexpr int experience_duration_sec = 10;
constexpr int photo_countdown_sec = 5;

class VideoTracker{
public:

    /*! @brief Default constructor */
    VideoTracker();

    /*! @brief Threaded function - check the Camera::capture() / Audiolizer::capture() for more details */
    void capture();

    /*! @brief Updates the tracking results */
    bool update(RegionOfInterest&, cv::Mat&);

    // /*! @brief Pulses whether a new tracking update has been emerged */
    // bool pattern_locked();

    // /*! @brief Pulses whether a new camera frame has been captured */
    // bool tickTock();

private:

    Config &cfg = Config::getInstance();

    bool _tracking_updated();
    void _initialize_tracker();
    void _show_timer(const int);

    std::atomic<bool> trackingToggle;
    int toggleTrack;
    bool atomicChange;

    Camera camera;
    int countdown_ms;
    Timer t1,t2;
    cv::Ptr<cv::Tracker> tracker;
    RegionOfInterest boxCenter;
    cv::Mat ROI,currFrame;
    cv::Rect centerBox;
    cv::Rect boundingBox;
    
    std::atomic<int> currboxCenter_x,currboxCenter_y,currboxCenter_w,currboxCenter_h;
    std::atomic<bool> patternlocked;

    // const int waiting_ms;

};

#endif