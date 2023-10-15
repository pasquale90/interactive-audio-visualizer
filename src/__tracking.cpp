#include "__tracking.h"


Tracking::Tracking(){
}

Tracking::~Tracking(){

}

void Tracking::setConfig(const Config& cfg){
    
    // bufferSize=buffer_size;
    camera.setConfig(cfg);
    camera.display_config();

    ROIoffset=cfg.roiOffset;
    ROIw8sec = cfg.roiSec;

}

void Tracking::_capture(){
    camera.capture();
}

bool Tracking::get_frame_elapsed(){
    return camera.get_frame_elapsed();
}

void Tracking::display_config(){
    std::cout<<"Tracking Config"<<std::endl;
    std::cout<<"ROIoffset "<<ROIoffset<<std::endl;
    std::cout<<"ROIw8sec "<<ROIw8sec<<std::endl;
}
