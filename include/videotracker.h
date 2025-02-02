#ifndef TRACKING_H
#define TRACKING_H

#include <opencv2/tracking.hpp>
#include "config.h"
struct RegionOfInterest;

/*!
 * @brief A class responsible for tracking objects in the camera feed.
 * @note This class uses OpenCV's built-in tracker API for object tracking.
 */
class VideoTracker{
public:

    /*! @brief Default constructor */
    VideoTracker();

    /*!
     * @brief Function to initialize the tracker with a given image.
     * @param[in] const cv::Mat& - Image containing the object to be tracked.
     * @return void
    */
    void initializeTracker(const cv::Mat&);

    /*!
     * @brief Function to track the object in the given image.
     * @param[in] const cv::Mat& - Image containing the object to be tracked.
     * @param[in] RegionOfInterest& - Region of interest where the object should be tracked.
     * @return bool - True if the object is successfully tracked, false otherwise.
    */
    bool trackObject(const cv::Mat&, RegionOfInterest&);
    
private:

    Config &cfg = Config::getInstance();

    cv::Ptr<cv::Tracker> tracker;
    cv::Rect centerBox;
    cv::Rect boundingBox;

};

#endif