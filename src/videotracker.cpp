#include "videotracker.h"


VideoTracker::VideoTracker(){
    patternlocked=false;
    
//TEMPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
    cv::namedWindow("2");
}

VideoTracker::~VideoTracker(){
//TEMPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
    cv::destroyWindow("2");
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
    std::cout<<ROIoffset*2<<"=="<<w<<"=="<<h<<std::endl;
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

    trackingToggle=0;
}

void VideoTracker::display_config(){
    std::cout<<"VideoTracker Config"<<std::endl;
    std::cout<<"ROIoffset "<<ROIoffset<<std::endl;
    std::cout<<"ROIw8sec "<<ROIw8sec<<std::endl;
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

                imshow("2", currFrame);

                cv::waitKey(1);
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

            patternlocked=true;
        }
    }else{
        bool ok;

        if (framecounter==0){
            std::cout<<"first occurence --> init tracker here"<<framecounter<<std::endl;
            tracker->init(currFrame, boundingBox);
            // trackerInitialized=true;
        }

        ok = tracker->update(currFrame, boundingBox);         

        if (framecounter<=((-10)*fps) || boundingBox.x<=0 || boundingBox.y<=0 || boundingBox.width<=0 || boundingBox.height<=0 || (boundingBox.x+boundingBox.width)>=W || (boundingBox.y+boundingBox.height)>=H ) {
            patternlocked=false;
            framecounter=ROIw8sec*fps;
            boundingBox = centerBox;
            

            std::cout<<"Creating a new trackerrrrrrrrrrrrrrrrrrrr"<<std::endl;
            tracker->clear();
            cv::Ptr<cv::TrackerCSRT> trackerNew = cv::TrackerCSRT::create();
            tracker = trackerNew;
        }//break; //locker=false;
        else{

            if (ok)
            {
                std::cout<<"\n\nVideo::tracker :: sucessssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\n\n"<<std::endl;
                // Tracking success : Draw the tracked object
                rectangle(currFrame, boundingBox, cv::Scalar( 255, 0, 0 ), 2, 1 );
                trackingToggle=!trackingToggle;

                currboxCenter_x.store(boundingBox.x);
                currboxCenter_y.store(boundingBox.y);
            }
            else
            {
                std::cout<<"Tracking failed --> ok is not true "<<std::endl;
                // Tracking failure detected.
                putText(currFrame, "Tracking failure detected", cv::Point(100,80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,255),2);
            }


            // printf("boundingBox center (%f,%f) with width %f and height %f \n",boundingBox.x,boundingBox.y,boundingBox.width,boundingBox.height);

            // cv::Rect temp(boundingBox.x,boundingBox.y,LTRBwhCxCy[4],LTRBwhCxCy[5]);
            // cv::Mat tempROI(currFrame, temp);
            // // Copy the data into new matrix
            // tempROI.copyTo(ROI);

            // imshow("2", currFrame);

            cv::Mat tempROI(currFrame, boundingBox);
            // Copy the data into new matrix
            tempROI.copyTo(ROI);

            imshow("2", currFrame);
            cv::waitKey(1);
            // std::cout<<"Start tracking the cropped from frame"<<std::endl;

            framecounter--; //++;
        }
            
    }  

}

bool VideoTracker::update(std::pair<int,int> &roi_center,cv::Mat& roi){
    /***
     * Updates the roi and the (global) roi center. 
     * Returns bool - Applies control the return values (i.e. if no changes occured by a visual stimulus
    */

    // imshow("2", currFrame);

    std::cout<<"VideoTracker::update ROI empty "<<ROI.empty()<<" patternlocked="<<patternlocked<<std::endl;
    ROI.copyTo(roi);
    if (patternlocked){
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

void VideoTracker::_initialize_tracker(){
    std::cout<<"Here we initialize the trackerrrrrrrrrrrr"<<std::endl;
    // cv::Rect centerBox(roiLTRB[0],roiLTRB[0],ROIoffset,ROIoffset); // could be initialized as a member variable to reduce computational load
    tracker->init(currFrame, boundingBox);
}

void VideoTracker::_init_timer(){
    framecounter=ROIw8sec*camera.get_fps();                 // restart timer
}