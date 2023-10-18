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
    frameToggle=false;
}

Camera::Camera(){
    camW=0;
    camH=0;
    fps=0;
    // cv::namedWindow("Window");
    frameToggle=0;
    // std::cout<<"Camera initialized 1"<<std::endl;
}

void Camera::setConfig(const Config& cfg){
    camW=cfg.camResW; 
    camH=cfg.camResH;
    fps=cfg.camfps;
    std::cout<<"camW "<<camW<<std::endl;
    std::cout<<"camH "<<camH<<std::endl;
    std::cout<<"fps "<<fps<<std::endl;
    initialize_camera();

    // cv::Mat img(camH,camW, CV_8UC3);
    // cv::Mat frame(camH,camW, CV_8UC3);
    // frame=img.clone();

    // frameCount=0;

    cv::Mat frame(camH,camW,CV_8UC3);
    // std::cout<<"Camera initialized 2"<<std::endl;

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

// Camera::Camera(){
//     Config cfg;
//     camW=cfg.camResW;
//     camH=cfg.camResH; 
//     fps=cfg.fps;
// }

void Camera::display_config(){
    std::cout<<"camW "<<camW<<std::endl;
    std::cout<<"camH "<<camH<<std::endl;
    std::cout<<"fps "<<fps<<std::endl;
}

Camera::~Camera(){
    frame.release();
    cap.release();
    std::cout<<"Camera object destructed"<<std::endl;
}


bool Camera::get_frame_elapsed(){

    atomicChange = frameToggle.load();
    // std::cout<<"atomicChange - toggleFrame "<<atomicChange<<" - "<<toggleFrame<<std::endl;
    if (atomicChange!=toggleFrame){     // process the current input from camera
        // std::cout<<"toggle? "<<"Yes!!"<<std::endl;
        toggleFrame=atomicChange;
        return true;
    }else{
        // std::cout<<"toggle? "<<"No :((((((((((((((((((((("<<std::endl;
        return false;
    }

}

void Camera::get_current_frame(cv::Mat& f){
    f=frame;
}

int Camera::get_fps(){
    return fps;
}

int Camera::get_actual_fps(){
    return (int)proper_fps;
}

void Camera::capture(){

    // cv::namedWindow("Window");

    // cv::Mat frame;

    while(1){
        cap.read(frame);

        if(!(frame.empty())){
            // imshow("Window", frame);
            // frameToggle=!frameToggle;
            // int toggle=frameToggle.load(std::memory_order_relaxed);
            // toggle++;
            // frameToggle=++frameToggle; //.fetch_add(1);//store(toggle,std::memory_order_relaxed); 

            // ftog=!ftog;
            // std::cout<<"togle togle togle "<<toggle<<"-- "<<frameToggle<<"-- "<<ftog<<std::endl;
            // bool tog=!frameToggle;
            frameToggle=!frameToggle;
            // frameToggle.fetch_add(1);
            // this->set_frame(frameToggle.load());
            std::cout<<"************************************************************************captured! ************************************************************************"<<frameToggle.load()<<std::endl;

            // toggle=frameToggle.load();


            
        // int R=(int)frame.at<cv::Vec3b>(0,0)[0];
        // int B=(int)frame.at<cv::Vec3b>(0,0)[1];
        // int G=(int)frame.at<cv::Vec3b>(0,0)[2];
        // std::cout<<"Pixel check at center "<<B<<","<<G<<","<<R<<std::endl; 
        }

        // cv::waitKey(1);
    }
}
