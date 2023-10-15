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

    // cv::Mat img(camH,camW, CV_8UC3);
    // cv::Mat frame(camH,camW, CV_8UC3);
    // frame=img.clone();

    // frameCount=0;

    cv::Mat frame(camH,camW,CV_8UC3);
    // cv::namedWindow("Window");

    // frameToggle=0;
    // // frameToggle=1;// (1);
    // toggle=false;
    // ftog=false;
    // ftog = ATOMIC_VAR_INIT(false);
    frameToggle=false;
}


Camera::Camera(int camResW,int camResH, int camfps):
                                camW(camResW), 
                                camH(camResH), 
                                fps(camfps){
    std::cout<<"camW "<<camW<<std::endl;
    std::cout<<"camH "<<camH<<std::endl;
    std::cout<<"fps "<<fps<<std::endl;
    initialize_camera();

    // cv::Mat img(camH,camW, CV_8UC3);
    // cv::Mat frame(camH,camW, CV_8UC3);
    // frame=img.clone();

    // frameCount=0;

    cv::Mat frame(camH,camW,CV_8UC3);
    cv::namedWindow("Window");

    frameToggle=0;
    // // frameToggle=1;// (1);
    // toggle=false;
    // // ftog=false;
    // // ftog = ATOMIC_VAR_INIT(false);
    // frameToggle.store(0);
}

Camera::Camera(){
    camW=0;
    camH=0;
    fps=0;
    cv::namedWindow("Window");
    frameToggle=0;
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
    double proper_fps = cap.get(cv::CAP_PROP_FPS);
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
    cap.release();
    std::cout<<"Camera object destructed"<<std::endl;
}

// void Camera::set_frame(bool val){
//     toggle=val;
// }

bool Camera::get_frame(){
    // return frameToggle.load();
    // std::cout<<"get_frame -------------------> "<<this->frameToggle.load()<<" --- "<<std::endl;
    return frameToggle.load();
    // toggle = frameToggle.load(std::memory_order_seq_cst);
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


            // ftog.fetch_xor(toggle,std::memory_order_seq_cst );
        int R=(int)frame.at<cv::Vec3b>(0,0)[0];
        int B=(int)frame.at<cv::Vec3b>(0,0)[1];
        int G=(int)frame.at<cv::Vec3b>(0,0)[2];
        std::cout<<"Pixel check at center "<<B<<","<<G<<","<<R<<std::endl; 
        }

        // cv::waitKey(1);
    }
}