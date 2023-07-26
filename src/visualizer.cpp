#include "visualizer.h"

Visualizer::Visualizer(int width,int height){
    cv::namedWindow("Visualizer",cv::WINDOW_AUTOSIZE);
    cv::Mat img(width,height, CV_8UC3, cv::Scalar(100,200,0));
    videoframe = img;
}

Visualizer::Visualizer(){
}

Visualizer::~Visualizer(){
    cv::destroyWindow("Visualizer");  
}

int Visualizer::empty_visualizer(){
    if (videoframe.empty()) 
    {
        std::cout << "\n Image not created. You have done something wrong. \n";
        return -1;    // Unsuccessful.
    }

    cv::imshow("Visualizer", videoframe);//Showing the video//
    cv::waitKey(1); //Allowing 1 milliseconds frame processing time
    return 0;
}

int Visualizer::stream_frames(float *in){
    int success=empty_visualizer();

    // audio_visualizer(); #
    // user_visualizer();  #
}