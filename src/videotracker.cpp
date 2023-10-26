#include "videotracker.h"


VideoTracker::VideoTracker(){
    trackingToggle.store(false);
    patternlocked.store(false);
    // cv::namedWindow("2");
}

VideoTracker::~VideoTracker(){
    // cv::destroyWindow("2");
}

void VideoTracker::setConfig(const Config& cfg){
    
    camera.setConfig(cfg);
    camera.display_config();

    radius=cfg.radius;
    ROIw8sec = cfg.roiSec;
    framecounter=ROIw8sec*camera.get_fps();

    W=cfg.camResW;
    H=cfg.camResH;
    fps = camera.get_fps();
    
    boxCenter={W/2,H/2};
    // previous_boxCenter=current_boxCenter;

    cv::Rect temp((W/2)-radius,(H/2)-radius,radius*2,radius*2);
    centerBox=temp;
    boundingBox=temp;
    
    if (cfg.trackingAlg == 0) //"CSRT"
        tracker = cv::TrackerCSRT::create();
    if (cfg.trackingAlg == 1) //"MOSSE"
        tracker = cv::TrackerMOSSE::create();
    if (cfg.trackingAlg == 2) //"BOOSTING"
        tracker = cv::TrackerBoosting::create();
}

void VideoTracker::display_config(){
    std::cout<<"VideoTracker Config"<<std::endl;
    std::cout<<"radius "<<radius<<std::endl;
    std::cout<<"ROIw8sec "<<ROIw8sec<<std::endl;
}

bool VideoTracker::tickTock(){
    return camera._frame_elapsed();
}

bool VideoTracker::_pattern_locked(){
    return patternlocked.load();
}

bool VideoTracker::_tracking_updated(){

    atomicChange = trackingToggle.load();
    if (atomicChange!=toggleTrack){     
        toggleTrack=atomicChange;
        return true;
    }else{
        return false;
    }
}

void VideoTracker::_capture(){
    /***
     * Updates the roi and the (global) roi center. 
    */
    bool frameElapsed = camera.capture(currFrame);

    if(!patternlocked.load()){

        std::cout<<"time counter "<<framecounter<<std::endl;
    
        if (framecounter>0){
            circle( currFrame,
                    cv::Point((W/2),(H/2)),
                    radius,
                    cv::Scalar( 0, 255, 0 ),
                    thickness=1,
                    cv::LINE_8);

            cv::Mat tempROI(currFrame, centerBox); // access box information                
            tempROI.copyTo(ROI); // Copy the data into new matrix

            // PREPROCESS

            if(BG.empty()){ // --------------------------------------------------> that is the first frame
                ROI.copyTo(BG);
            }else{

                cv::putText(currFrame, //target image
                    std::to_string(framecounter/fps), //text
                    cv::Point(10, currFrame.rows / 2), //top-left position
                    cv::FONT_HERSHEY_DUPLEX,
                    1.0,
                    CV_RGB(118, 185, 0), //font color
                    2);

                // imshow("2", currFrame);
                // cv::waitKey(1);
            }
            framecounter--;

        }else if(framecounter==0){     // if similarity with the background is high, restart the timer! --> disabled
        
            // // Calculate the L2 relative error between current ROI and the BG.
            // double errorL2 = norm( ROI, BG, cv::DIST_L2 );
            // // Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
            // double L2difference = errorL2 / (double)( ROI.rows * ROI.cols );
            // std::cout<<"similarity difference "<<L2difference<<" similarity_threshold "<<similarity_threshold<<std::endl;

            // if (L2difference < similarity_threshold){ // if current roi is similar to BG --> low L2
            //     framecounter=fps*5;
            // }else patternlocked=true;

            patternlocked.store(true);
        }
    }else{
        bool ok;

        if (framecounter==0){
            std::cout<<"first occurence --> init tracker here"<<framecounter<<std::endl;
            tracker->init(currFrame, boundingBox);
        }

        ok = tracker->update(currFrame, boundingBox);         

        if (framecounter<=((-10)*fps) || boundingBox.x<=0 || boundingBox.y<=0 || boundingBox.width<=0 || boundingBox.height<=0 || (boundingBox.x+boundingBox.width)>=W || (boundingBox.y+boundingBox.height)>=H ) {
            patternlocked.store(false);
            framecounter=ROIw8sec*fps;
            boundingBox = centerBox;
            
            // Creating a new tracker
            tracker->clear();
            cv::Ptr<cv::TrackerCSRT> trackerNew = cv::TrackerCSRT::create();
            tracker = trackerNew;
        }
        else{

            if (ok)
            {
                // Tracking success : Draw the tracked object
                // rectangle(currFrame, boundingBox, cv::Scalar( 255, 0, 0 ), 2, 1 ); // only for testing
                trackingToggle=!trackingToggle;

                currboxCenter_x.store(boundingBox.x);
                currboxCenter_y.store(boundingBox.y);
            }
            else
            {
                // Tracking failure detected.
                putText(currFrame, "Tracking failure detected", cv::Point(100,80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,255),2);
            }

            // keep the initial box width and height
            // cv::Rect temp(boundingBox.x,boundingBox.y,radius*2,radius*2);
            // cv::Mat tempROI(currFrame, temp);
            // tempROI.copyTo(ROI);

            cv::Mat tempROI(currFrame, boundingBox);
            tempROI.copyTo(ROI);
            framecounter--; //++;
            // imshow("2", currFrame);
        }
            
    }  

}

bool VideoTracker::update(std::pair<int,int> &roi_center,cv::Mat& roi){
    /***
     * Updates the roi and the (global) roi center. 
     * Returns bool - Applies control the return values (i.e. if no changes occured by a visual stimulus
    */

    currFrame.copyTo(roi);
    if (patternlocked.load()){
        roi_center.first=currboxCenter_x.load();
        roi_center.second=currboxCenter_y.load();
    }else{
        roi_center.first=boxCenter.first;
        roi_center.second=boxCenter.second;
    }
    return _tracking_updated();
}

bool VideoTracker::_check_similarity(){
    // Calculate the L2 relative error between current ROI and the BG.
    double errorL2 = norm( ROI, BG, cv::DIST_L2 );
    // Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
    double L2difference = errorL2 / (double)( ROI.rows * ROI.cols );
    std::cout<<"similarity difference"<<L2difference<<" similarity_threshold "<<similarity_threshold<<std::endl;

    if (L2difference < similarity_threshold){ // if current roi is similar to BG --> low L2
        return true;
    }else{                                  // else initialize tracking
        return false;
    }
}

void VideoTracker::_init_timer(){
    framecounter=ROIw8sec*camera.get_fps();                 // restart timer
}