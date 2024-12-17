#include "videotracker.h"

VideoTracker::VideoTracker(){
    trackingToggle.store(false);
    patternlocked.store(false);

    // if (cfg.iavconf.trigger == "Auto"); //  @TODO
    countdown_ms = photo_countdown_sec*1000;
    t1.setTimer(photo_countdown_sec);
    t2.setTimer(experience_duration_sec);

    boxCenter.centerX=cfg.camconf.camResW.load()/2;
    boxCenter.centerY=cfg.camconf.camResH.load()/2;
    boxCenter.volumeW=cfg.iavconf.roiRadius;
    boxCenter.volumeH=cfg.iavconf.roiRadius;

    int radius=cfg.iavconf.roiRadius;
    int W=cfg.camconf.camResW.load();
    int H=cfg.camconf.camResH.load();
    cv::Rect temp((W/2)-radius,(H/2)-radius,radius*2,radius*2);
    centerBox=temp;
    boundingBox=temp;
    
    _initialize_tracker();
}

void VideoTracker::_initialize_tracker(){
    if (cfg.iavconf.trackingAlg == "CSRT") {
        tracker = cv::TrackerCSRT::create();
    } else if (cfg.iavconf.trackingAlg == "KFC") {
        tracker = cv::TrackerKCF::create();
    }else if (cfg.iavconf.trackingAlg == "BOOSTING") {
        tracker = cv::TrackerBoosting::create();
    }
}

// bool VideoTracker::tickTock(){
//     return camera.frame_elapsed();
// }

// bool VideoTracker::pattern_locked(){
//     return patternlocked.load();
// }

bool VideoTracker::_tracking_updated(){

    atomicChange = trackingToggle.load();
    if (atomicChange!=toggleTrack){     
        toggleTrack=atomicChange;
        return true;
    }else{
        return false;
    }
}


void VideoTracker::_show_timer(const int millisecondsElapsed) {
    
    int x = currFrame.cols / 2;
    int y = currFrame.rows / 2;

    int radius = cfg.iavconf.roiRadius;  
    int thickness = 3;
    double angle = (millisecondsElapsed / static_cast<double>(countdown_ms)) * 360.0;
    cv::circle(currFrame, cv::Point(x, y), radius, CV_RGB(245, 245, 245), thickness);

    // int r = 255 - (millisecondsElapsed * 5) % 255;  // Gradually change the color to orange/yellow
    // int g = (millisecondsElapsed * 2) % 255;
    // int b = (millisecondsElapsed * 4) % 255;
    int r = static_cast<int>(127.5 * (1 + sin(angle * M_PI / 180.0)));  // Sinusoidal for red
    int g = static_cast<int>(127.5 * (1 + sin((angle + 120) * M_PI / 180.0)));  // Sinusoidal for green
    int b = static_cast<int>(127.5 * (1 + sin((angle + 240) * M_PI / 180.0)));  // Sinusoidal for blue

    cv::ellipse(currFrame, cv::Point(x, y), cv::Size(radius, radius), 0, -90, -90 + angle, CV_RGB(r, g, b), thickness);

}

static int framecounter = 0;
void VideoTracker::capture(){

    /***
     * Updates the video frames and implements the detection pipeline. 
    */

    while(true){
        
        bool frameElapsed = camera.capture(currFrame);

        framecounter++;

        if (!frameElapsed){
            break;
        }
        
        int W = cfg.camconf.camResW.load();
        int H = cfg.camconf.camResH.load();

        int millisecondsElapsed;

        // if there s not pattern yet.
        if(!patternlocked.load()){

            bool countDown = t1.getCurrentTime(millisecondsElapsed);
            // std::cout<<"t1 countDown "<<countDown<< " seconds "<<millisecondsElapsed<<" patternlocked.load() "<<patternlocked.load()<<std::endl;

            // if still waiting for the automatic trigger
            if(!countDown){

                _show_timer(millisecondsElapsed);
                // temporarily show frame...
                // imshow("2", currFrame);
                // cv::waitKey(1);

            }else // if trigger is activated. 
            {   cv::Mat tempROI(currFrame, centerBox); // access box information                
                tempROI.copyTo(ROI); // Copy the data into new matrix
                patternlocked.store(true);
            }
        }else{
        // }else if (framecounter%3==0){ // --> this will be used to skip frames 
            bool ok;

            // if the time allowed for the iav experience has elapsed
            if (t1.isTimerFinished()){
                // std::cout<<"first occurence --> init tracker here"<<std::endl;
                tracker->init(currFrame, boundingBox);
                t2.setTimer(experience_duration_sec);
                t1.pauseTimer();
            }

            bool countDown = t2.getCurrentTime(millisecondsElapsed);

            ok = tracker->update(currFrame, boundingBox);         

            // prevent out of lost tracking / time elapsed / bounds tracking
            if (countDown || boundingBox.x<=0 || boundingBox.y<=0 || boundingBox.width<=0 || boundingBox.height<=0 || (boundingBox.x+boundingBox.width)>=W || (boundingBox.y+boundingBox.height)>=H ) {
                
                patternlocked.store(false);
                t1.setTimer(5);

                boundingBox = centerBox;
                
                // Creating a new tracker
                tracker->clear();
                _initialize_tracker();
                tracker->init(currFrame, boundingBox);
            }
            else{ // during  successfull tracking
                if (ok){

                    trackingToggle=!trackingToggle;

                    currboxCenter_x.store(static_cast<int>(boundingBox.x + boundingBox.width/2));
                    currboxCenter_y.store(static_cast<int>(boundingBox.y + boundingBox.height/2));
                    currboxCenter_w.store(static_cast<int>(boundingBox.width));
                    currboxCenter_h.store(static_cast<int>(boundingBox.height));
        
                }
            }

            // temp draw detection
            // int radius = cfg.iavconf.roiRadius;
            // cv::Rect temprect(boundingBox.x,boundingBox.y,radius*2,radius*2);
            // cv::rectangle(currFrame, temprect, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
            // imshow("1", currFrame);
            // cv::waitKey(1);
        } 
    }
}

bool VideoTracker::update(RegionOfInterest &roi_center,cv::Mat& frame){
    /***
     * Updates the roi and the (global) roi center. 
     * Returns bool - Applies control the return values (i.e. if no changes occured by a visual stimulus
    */

    if (patternlocked.load()){
        roi_center.centerX.store(currboxCenter_x.load());
        roi_center.centerY.store(currboxCenter_y.load());
        roi_center.volumeW.store(currboxCenter_w.load());
        roi_center.volumeH.store(currboxCenter_h.load());
        if (!frame.empty()) {
            frame.release();
        }

    }else{
        currFrame.copyTo(frame);
        roi_center.centerX.store(-1);
        roi_center.centerY.store(-1);
        roi_center.volumeW.store(-1);
        roi_center.volumeH.store(-1);
    }

    return _tracking_updated();
}