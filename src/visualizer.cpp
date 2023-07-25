#include "visualizer.h"

Visualizer::Visualizer(int width,int height){
    cv::namedWindow("Visualizer",cv::WINDOW_AUTOSIZE);
    cv::Mat img(width,height, CV_8UC3, cv::Scalar());
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
    std::cout<<"visualizer.cpp : I need a thread to run in parallel"<<std::endl;
    //
    //https://www.geeksforgeeks.org/thread-functions-in-c-c/
    //https://www.google.com/search?channel=fs&client=ubuntu-sn&q=opencv+run+in++pthread
    //https://stackoverflow.com/questions/35237460/using-pthread-in-simple-c-opencv-project
    //https://stackoverflow.com/questions/1151582/pthread-function-from-a-class
    //https://stackoverflow.com/questions/6990888/c-how-to-create-thread-using-pthread-create-function
    cv::imshow("Visualizer", videoframe);//Showing the video//
    return 0;
    // char c = (char)cv::waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
    // if (c == 27){ //If 'Esc' is entered break the loop//
    //     return;
    // }
}
void Visualizer::stream_frames(){
    int success=empty_visualizer();

    // audio_visualizer(); #
    // user_visualizer();  #
}