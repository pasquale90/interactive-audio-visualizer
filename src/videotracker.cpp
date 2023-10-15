#include "videotracker.h"


VideoTracker::VideoTracker(){
}

VideoTracker::~VideoTracker(){

}

void VideoTracker::setConfig(const Config& cfg){
    
    // bufferSize=buffer_size;
    camera.setConfig(cfg);
    camera.display_config();

    ROIoffset=cfg.roiOffset;
    ROIw8sec = cfg.roiSec;

}

void VideoTracker::_capture(){
    camera.capture();
}

bool VideoTracker::get_frame_elapsed(){
    return camera.get_frame_elapsed();
}

void VideoTracker::display_config(){
    std::cout<<"VideoTracker Config"<<std::endl;
    std::cout<<"ROIoffset "<<ROIoffset<<std::endl;
    std::cout<<"ROIw8sec "<<ROIw8sec<<std::endl;
}
