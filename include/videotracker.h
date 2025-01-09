#ifndef TRACKING_H
#define TRACKING_H

#include <opencv2/tracking.hpp>
#include "config.h"
struct RegionOfInterest;

class VideoTracker{
public:

    /*! @brief Default constructor */
    VideoTracker();

    void initializeTracker(const cv::Mat&);

    bool trackObject(const cv::Mat&, RegionOfInterest&);
    
private:

    Config &cfg = Config::getInstance();

    cv::Ptr<cv::Tracker> tracker;
    cv::Rect centerBox;
    cv::Rect boundingBox;

};

#endif