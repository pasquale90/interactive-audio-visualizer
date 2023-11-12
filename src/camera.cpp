// https://learnopencv.com/object-tracking-using-opencv-cpp-python/



#include "camera.h"


Camera::Camera(const Config& cfg):
                                camW(cfg.camResW), 
                                camH(cfg.camResH), 
                                fps(cfg.camfps){
    std::cout<<"camW "<<camW<<std::endl;
    std::cout<<"camH "<<camH<<std::endl;
    std::cout<<"fps "<<fps<<std::endl;
    initialize_camera();
    cv::Mat frame(camH,camW,CV_8UC3);
    frameToggle.store(false);
    toggleFrame=false;
}

Camera::Camera(){
    camW=0;
    camH=0;
    fps=0;
    // cv::namedWindow("Window");
    frameToggle.store(false);
    // std::cout<<"Camera initialized 1"<<std::endl;
}

void Camera::setConfig(const Config& cfg){
    camW=cfg.camResW; 
    camH=cfg.camResH;
    fps=cfg.camfps;
    initialize_camera();
    cv::Mat frame(camH,camW,CV_8UC3);
    toggleFrame=false;
}


void Camera::initialize_camera(){
    int deviceID = 0; // 0 = open default camera
    int apiID = 0; // 0 = autodetect default API - otherwise use --> cv::CAP_GSTREAMER
    cap.open(deviceID, apiID);     // cap.open(0);
    int w=cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int h=cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, camW);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, camH);
    cap.set(cv::CAP_PROP_FORMAT, CV_8UC3 );
    proper_fps = cap.get(cv::CAP_PROP_FPS);
    // std::cout<<"Proper fps "<<proper_fps<<std::endl;
    // std::cout<<"Proper w "<<w<<std::endl;
    // std::cout<<"Proper h "<<h<<std::endl;
    cap.set(cv::CAP_PROP_FPS, fps);
    if( !cap.isOpened() )
    {
        std::cerr << "***Could not initialize capturing...***\n";
    }else std::cout<<"Camera initialized succesfully"<<std::endl;

}

void Camera::display_config(){
    std::cout<<"Camera config :: camW "<<camW<<std::endl;
    std::cout<<"Camera config :: camH "<<camH<<std::endl;
    std::cout<<"Camera config :: fps "<<fps<<std::endl;
}

Camera::~Camera(){
    frame.release();
    cap.release();
    std::cout<<"Camera object destructed"<<std::endl;
}

int Camera::get_fps(){
    return fps;
}

int Camera::get_actual_fps(){
    return (int)proper_fps;
}

bool Camera::frame_elapsed(){

    atomicChange = frameToggle.load();
    
    if (frameToggle.load()!=toggleFrame){     // process the current input from camera
        toggleFrame=atomicChange;
        return true;
    }else
        return false;

}
void Camera::get_current_frame(cv::Mat& f){
    f=frame;
}

bool Camera::capture(cv::Mat& frame){

    cap.read(frame);

    if(!(frame.empty())){
        frameToggle.store(!frameToggle.load());
        // std::cout<<"************************************************************************captured! ************************************************************************"<<frameToggle.load()<<std::endl;
        return true;
    }
    return false;
}