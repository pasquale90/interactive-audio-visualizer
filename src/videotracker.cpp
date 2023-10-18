#include "videotracker.h"


VideoTracker::VideoTracker(){
    patternlocked=false;
    
//TEMPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
    cv::namedWindow("Window2");
}

VideoTracker::~VideoTracker(){
//TEMPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
    cv::destroyWindow("Window2");
}

void VideoTracker::setConfig(const Config& cfg){
    
    camera.setConfig(cfg);
    camera.display_config();

    // bufferSize=cfg.bufferSize;
    ROIoffset=cfg.roiOffset;
    ROIw8sec = cfg.roiSec;
    framecounter=ROIw8sec*camera.get_fps();

    W=cfg.camResW;
    H=cfg.camResH;
    fps = camera.get_fps();
    
    int l = (W/2)-ROIoffset;
    int t = (H/2)-ROIoffset;
    int r = (W/2)+ROIoffset;
    int b = (H/2)+ROIoffset;
    int w = r-l;
    int h = b-t;
    int LTRBwhCxCy[8] = {l,t,r,b,w,h,W/2,H/2};
    
    boxCenter={W/2,H/2};
    // previous_boxCenter=current_boxCenter;

    cv::Rect temp(LTRBwhCxCy[0],LTRBwhCxCy[1],LTRBwhCxCy[4],LTRBwhCxCy[5]);
    centerBox=temp;
    boundingBox=temp;
    std::cout<<"boxCenter "<<boxCenter.first<<","<<boxCenter.second<<std::endl;
    
    if (cfg.trackingAlg == 0) //"CSRT"
        tracker = cv::TrackerCSRT::create();
    if (cfg.trackingAlg == 1) //"MOSSE"
        tracker = cv::TrackerMOSSE::create();
    if (cfg.trackingAlg == 2) //"BOOSTING"
        tracker = cv::TrackerBoosting::create();
}

void VideoTracker::_capture(){
    camera.capture();
}

bool VideoTracker::get_frame_elapsed(){
    return camera.get_frame_elapsed();
}

void init_ROI_center(std::pair<int,int>&){

}

void VideoTracker::display_config(){
    std::cout<<"VideoTracker Config"<<std::endl;
    std::cout<<"ROIoffset "<<ROIoffset<<std::endl;
    std::cout<<"ROIw8sec "<<ROIw8sec<<std::endl;
}


void VideoTracker::_cropROI(bool initBG){
    // void Camera::cropROI(cv::Mat &ROI, int left, int top, int width, int height){
    int thickness=1;
    circle(currFrame,                      
        cv::Point(boxCenter.first,boxCenter.second),
        ROIoffset,
        cv::Scalar( 0, 255, 0 ),
        thickness=thickness,
        cv::LINE_8);

    // cv::Rect centerBox(roiLTRB[0],roiLTRB[0],ROIoffset,ROIoffset); // could be initialized as a member variable to reduce computational load
    cv::Mat roi(currFrame, centerBox);
    
    // Copy the data into new matrix
    if (initBG)
        roi.copyTo(BG);
    roi.copyTo(ROI);
}

void VideoTracker::_updateROI(){
    cv::Rect temp(boundingBox.x,boundingBox.y,ROIoffset,ROIoffset);
    cv::Mat detection(currFrame, temp);
    // Copy the data into new matrix
    detection.copyTo(ROI);
}

bool VideoTracker::update(std::pair<int,int> &roi_center,cv::Mat& roi){
    /***
     * Updates the roi and the (global) roi center. 
     * Returns bool - Applies control the return values (i.e. if no changes occured by a visual stimulus
    */

    camera.get_current_frame(currFrame);

    if(!patternlocked){

        std::cout<<"time counter "<<framecounter<<std::endl;
        
        if (framecounter>0){
        
            circle( currFrame,
                    cv::Point((W/2),H/2),
                    ROIoffset,
                    cv::Scalar( 0, 255, 0 ),
                    thickness=1,
                    cv::LINE_8);

            cv::Mat tempROI(currFrame, centerBox); // access box information                
            tempROI.copyTo(ROI); // Copy the data into new matrix

            // PREPROCESS
            // IF FIRST FRAME, WAIT FOR ONE MORE FOR HAVING SOMEONE TO PLAY WITH
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

                imshow("Window", currFrame);

                cv::waitKey(1);
            }
            framecounter--;

        }else if(framecounter==0){     // if similarity with the background is high, restart the timer!
        
            // // Calculate the L2 relative error between current ROI and the BG.
            // double errorL2 = norm( ROI, BG, cv::DIST_L2 );
            // // Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
            // double L2difference = errorL2 / (double)( ROI.rows * ROI.cols );
            // std::cout<<"similarity difference "<<L2difference<<" similarity_threshold "<<similarity_threshold<<std::endl;

            // if (L2difference < similarity_threshold){ // if current roi is similar to BG --> low L2
            //     framecounter=fps*5;
            // }else patternlocked=true;

            patternlocked=true;
        }
    }else{
        bool ok;

        if (framecounter==0){
            std::cout<<"first occurence --> init tracker here"<<framecounter<<std::endl;
            tracker->init(currFrame, boundingBox);
            // trackerInitialized=true;
        }else{
            ok = tracker->update(currFrame, boundingBox);
        }         

        if (framecounter<=((-10)*fps) || boundingBox.x<=0 || boundingBox.y<=0 || boundingBox.width<=0 || boundingBox.height<=0 || (boundingBox.x+boundingBox.width)>=W || (boundingBox.y+boundingBox.height)>=H ) {
            patternlocked=false;
            framecounter=ROIw8sec*fps;
            boundingBox = centerBox;
            
            tracker->clear();
            cv::Ptr<cv::TrackerCSRT> trackerNew = cv::TrackerCSRT::create();
            tracker = trackerNew;
        }//break; //locker=false;
        else{

            if (ok)
            {
                // Tracking success : Draw the tracked object
                rectangle(currFrame, boundingBox, cv::Scalar( 255, 0, 0 ), 2, 1 );
            }
            else
            {
                std::cout<<"Tracking failed --> ok is not true "<<std::endl;
                // Tracking failure detected.
                putText(currFrame, "Tracking failure detected", cv::Point(100,80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,255),2);
            }

            printf("boundingBox center (%f,%f) with width %f and height %f \n",boundingBox.x,boundingBox.y,boundingBox.width,boundingBox.height);

            
            cv::Rect temp(boundingBox.x,boundingBox.y,LTRBwhCxCy[4],LTRBwhCxCy[5]);
            cv::Mat tempROI(currFrame, temp);
            // Copy the data into new matrix
            tempROI.copyTo(ROI);

            imshow("Window", currFrame);
            cv::waitKey(1);
            // std::cout<<"Start tracking the cropped from frame"<<std::endl;

            framecounter--; //++;
        }
            
    }   
                                             // if object of interest does not yet exists, and there is no detection yet ...
    //     if (BG.empty()){                                        // this is the first frame captured
    //         std::cout<<"Tracker::    ROIis empty -- > Try to get the the ROI as an cv::Mat"   <<std::endl;
    //         _cropROI(true); // initialize the BG
    //         BG.copyTo(ROI); // do not leave uninitialized
    //         BG.copyTo(prevROI); // do not leave uninitialized
    //         // current and previous roi_centers are initiliazed in the setConfig method - no need to initialize here.

    //     }else{  // after the first frame

    //         ROI.copyTo(prevROI);    // store previous ROI
    //         _cropROI(false);        //update ROI
                        
    //         // wait for user to interact with ROI
    //         patternlocked = _w84userInteraction();
    //         if (patternlocked){                                // if pattern is locked, run initintialize tracker
    //             _initialize_tracker();   // initialize tracker
    //         }

    //     }
    //     return false;
    // }else{
    //     // tracking
    //     std::cout<<"Now apply tracking!!!!!!!!!!!!"<<std::endl;
        
    //     bool ok = tracker->update(currFrame, boundingBox);
        
    //     if (ok){ // Tracking success
            
    //         std::cout<<"Okkkkkkkkk                                                          kkkkkkkkkkkkkkkkkkkkkkkkkkkkkko"<<std::endl;
    //         // temporarily draw box on frame -->>> erase
    //         // rectangle(currFrame, boundingBox, cv::Scalar( 255, 0, 0 ), 2, 1 );
    //         rectangle(currFrame, boundingBox, cv::Scalar( 255, 0, 0 ), 2, 1 );
    //         _updateROI();
            
    //     }else{
    //         // _w84immediateResponce(); // todo next --> this will debug empty boundingBox instances from failed trackings
    //         boundingBox.x= (boundingBox.x>boxCenter.first) ? boundingBox.x-2 : boundingBox.x+2;
    //         boundingBox.y= (boundingBox.y>boxCenter.second) ? boundingBox.y-2 : boundingBox.y+2;
    //         rectangle(currFrame, boundingBox, cv::Scalar( 255, 0, 0 ), 2, 1 );
    //         _updateROI();
    //     }
    //     currboxCenter.first=boundingBox.x+ROIoffset;
    //     currboxCenter.second=boundingBox.y+ROIoffset;
    //     if (currboxCenter.first == boxCenter.first && currboxCenter.second == boxCenter.second){
    //         patternlocked=false;
    //         std::cout<<"Pattern Lock is false again "<<std::endl;
    //         _init_timer();
    //         return false;
    //     }
        
    // }
    // //return by value
    // roi_center.first=currboxCenter.first;
    // roi_center.second=currboxCenter.second;
    // ROI.copyTo(roi);

    // cv::imshow("Window2",currFrame);
    // return true;
}

bool VideoTracker::_w84userInteraction(){
    /***
     * Waiting for someone to interact with 
     * currently only compares bg with the current ROI , only when framecounter equals to 0
     */




    std::cout<<"framecount "<<framecounter<<std::endl;

    if (framecounter>=0 && !patternlocked){           // time using framecount. We assume ROIw8sec equals to sec*fps
        
        cv::putText(currFrame,
                    std::to_string(framecounter/camera.get_fps()), //text
                    cv::Point(currboxCenter.first, currboxCenter.second - ROIoffset), // - 15), //slightly ~ 15 pixel above the ROI
                    cv::FONT_HERSHEY_DUPLEX,
                    1.0,CV_RGB(0, 0, 0),2);
        cv::imshow("Window2",currFrame);
        if (framecounter==0){

            if(_check_similarity()){                            // check the similarity between BG and ROI
                // _cropROI(true);                                         // update the background
                _init_timer();
            }else{
                framecounter=-1;                                // disable this part of code --> the if (framecounter>=0)
                return true;                                    // human interaction occured
            }     
        }
        framecounter--;
    }else{
        std::cout<<"VideoTracker::_w84userInteraction ERROR --> framecout<0 <-----------------------------------------------------FIX THIS"<<std::endl;
    }
    return false;
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

void VideoTracker::_initialize_tracker(){
    std::cout<<"Here we initialize the trackerrrrrrrrrrrr"<<std::endl;
    // cv::Rect centerBox(roiLTRB[0],roiLTRB[0],ROIoffset,ROIoffset); // could be initialized as a member variable to reduce computational load
    tracker->init(currFrame, boundingBox);
}

void VideoTracker::_init_timer(){
    framecounter=ROIw8sec*camera.get_fps();                 // restart timer
}