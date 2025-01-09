#include "videotracker.h"

#include "roi.h"

VideoTracker::VideoTracker(){

    int radius=cfg.iavconf.roiRadius;
    int W=cfg.camconf.camResW.load();
    int H=cfg.camconf.camResH.load();
    cv::Rect temp((W/2)-radius,(H/2)-radius,radius*2,radius*2);
    centerBox=temp;
    boundingBox=temp; 
}


void VideoTracker::initializeTracker(const cv::Mat& frame){
    if (cfg.iavconf.trackingAlg == "CSRT") {
        tracker = cv::TrackerCSRT::create();
    } else if (cfg.iavconf.trackingAlg == "KCF") {
        tracker = cv::TrackerKCF::create();
    }
    boundingBox = centerBox;
    tracker->init(frame, boundingBox);
}

bool VideoTracker::trackObject(const cv::Mat &frame , RegionOfInterest &trackingSig){

        bool trackingUpdated = tracker->update(frame, boundingBox);
        if (trackingUpdated){

    // @ comment : this gives the center x,y and the w and h
            trackingSig.centerX.store(static_cast<int>(boundingBox.x + boundingBox.width/2));
            trackingSig.centerY.store(static_cast<int>(boundingBox.y + boundingBox.height/2));
            trackingSig.volumeW.store(static_cast<int>(boundingBox.width));
            trackingSig.volumeH.store(static_cast<int>(boundingBox.height));
    // @ comment : this gives the TOPLEFT corner x,y and the w and h
            // trackingSig.centerX.store(static_cast<int>(boundingBox.x));
            // trackingSig.centerY.store(static_cast<int>(boundingBox.y));
            // trackingSig.volumeW.store(static_cast<int>(boundingBox.width));
            // trackingSig.volumeH.store(static_cast<int>(boundingBox.height));

        }
        return trackingUpdated;

}
