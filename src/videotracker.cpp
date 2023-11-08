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
    
    // boxCenter={W/2,H/2};
    boxCenter.centerX=W/2;
    boxCenter.centerY=H/2;
    boxCenter.volumeW=cfg.radius*2;
    boxCenter.volumeH=cfg.radius*2;

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

void VideoTracker::_show_timer(){
    std::string strtime = std::to_string(framecounter/fps);
    //----------------------------------------------------------- left
    int y = boxCenter.centerY + boxCenter.volumeH/2;
    int x = boxCenter.centerX - 2*boxCenter.volumeW;
    int b = 255 - currFrame.at<cv::Vec3b>( y , x )[0];
    int g = 255 - currFrame.at<cv::Vec3b>( y , x )[1];
    int r = 255 - currFrame.at<cv::Vec3b>( y , x )[2];
    cv::putText(currFrame, //target image
        strtime, //text
        cv::Point( x , y ), //almost center position
        cv::FONT_HERSHEY_DUPLEX,
        1.0,
        CV_RGB(r, g, b), //font color
        1);
    //----------------------------------------------------------- right
    y = currFrame.rows/2;                                   
    x = boxCenter.centerX + 2*boxCenter.volumeW;
    b = 255 - currFrame.at<cv::Vec3b>( y , x )[0];
    g = 255 - currFrame.at<cv::Vec3b>( y , x )[1];
    r = 255 - currFrame.at<cv::Vec3b>( y , x )[2];
    cv::putText(currFrame, //                                                                       
        strtime, //text
        cv::Point( x , y ), //almost center position
        cv::FONT_HERSHEY_DUPLEX,
        1.0,
        CV_RGB(r, g, b), //font color
        1);
        cv::putText(currFrame, //target image
        strtime, //text
        cv::Point( x , y ), //almost center position
        cv::FONT_HERSHEY_DUPLEX,
        1.0,
        CV_RGB(r, g, b), //font color
        1);
    //----------------------------------------------------------- top
    y = boxCenter.centerY - 2*boxCenter.volumeH;
    x = currFrame.cols/2;
    b = 255 - currFrame.at<cv::Vec3b>( y , x )[0];
    g = 255 - currFrame.at<cv::Vec3b>( y , x )[1];
    r = 255 - currFrame.at<cv::Vec3b>( y , x )[2];
    cv::putText(currFrame, //target image
        strtime, //text
        cv::Point( x , y ), //almost center position
        cv::FONT_HERSHEY_DUPLEX,
        1.0,
        CV_RGB(r, g, b), //font color
        1);
    //----------------------------------------------------------- bottom
    y = boxCenter.centerY + 2*boxCenter.volumeH;
    x = currFrame.cols/2;
    b = 255 - currFrame.at<cv::Vec3b>( y , x )[0];
    g = 255 - currFrame.at<cv::Vec3b>( y , x )[1];
    r = 255 - currFrame.at<cv::Vec3b>( y , x )[2];
    cv::putText(currFrame, //target image
        strtime, //text
        cv::Point( x , y ), //almost center position
        cv::FONT_HERSHEY_DUPLEX,
        1.0,
        CV_RGB(r, g, b), //font color
        1);
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
            // ...

            if(BG.empty()){ // --------------------------------------------------> that is the first frame
                ROI.copyTo(BG);
            }else{
                
                _show_timer();

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

                currboxCenter_x.store(boundingBox.x + boundingBox.width/2);
                currboxCenter_y.store(boundingBox.y + boundingBox.height/2);
                currboxCenter_w.store(boundingBox.width);
                currboxCenter_h.store(boundingBox.height);
                // std::cout<<"boundingBox <<"<<boundingBox.x<<"=="<<currboxCenter_x.load()<<std::endl;
                // std::cout<<"boundingBox <<"<<boundingBox.y<<"=="<<currboxCenter_y.load()<<std::endl;
                // std::cout<<"boundingBox <<"<<boundingBox.width<<"=="<<currboxCenter_w.load()<<std::endl;
                // std::cout<<"boundingBox <<"<<boundingBox.height<<"=="<<currboxCenter_h.load()<<std::endl;
            }   
            else
            {
                // currboxCenter_x.store(boxCenter.centerX);
                // currboxCenter_y.store(boxCenter.centerY);
                // currboxCenter_w.store(boxCenter.volumeW);
                // currboxCenter_h.store(boxCenter.volumeH);
                // Tracking failure detected.
                // putText(currFrame, "Tracking failure detected", cv::Point(100,80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,255),2);
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

bool VideoTracker::update(RegionOfInterest &roi_center,cv::Mat& roi){
    /***
     * Updates the roi and the (global) roi center. 
     * Returns bool - Applies control the return values (i.e. if no changes occured by a visual stimulus
    */

    currFrame.copyTo(roi);
    if (patternlocked.load()){
        roi_center.centerX=currboxCenter_x.load();
        roi_center.centerY=currboxCenter_y.load();
        roi_center.volumeW=currboxCenter_w.load();
        roi_center.volumeH=currboxCenter_h.load();
        // printf("\n\n currboxCenter  (%d,%d,%d,%d)  \n\n",currboxCenter_x.load(),currboxCenter_y.load(),currboxCenter_w.load(),currboxCenter_h.load());
    }else{
        roi_center = boxCenter;
        // roi_center.first=boxCenter.centerX;
        // roi_center.second=boxCenter.centerY;
        // roi_center.first=boxCenter.volumeW;
        // roi_center.second=boxCenter.volumeH;
        // printf("\n\n boxCenter  (%d,%d,%d,%d)  \n\n",boxCenter.centerX,boxCenter.centerY,boxCenter.volumeW,boxCenter.volumeH);
    }
    
    // std::cout<<"\n\n roi center "<<roi_center.centerX<<""<<" \n\n"<<std::endl;
    // printf("\n\n roi center       (%d,%d,%d,%d)  \n\n",roi_center.centerX,roi_center.centerY,roi_center.volumeW,roi_center.volumeH);

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