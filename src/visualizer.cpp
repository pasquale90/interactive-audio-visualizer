#include "visualizer.h"

Visualizer::Visualizer(int width,int height){
    cv::namedWindow("Visualizer",cv::WINDOW_AUTOSIZE);
    cv::Mat img(width,height, CV_8UC3); //, cv::Scalar(0,0,0));
    videoframe = img;
    // av=AudioVisualizer();
    R=0;
    G=0;
    B=0;
    
    incrR=3;
    incrG=1;
    incrB=7;
    ascR=true;
    ascG=true;
    ascB=true;

    update_counter=0;
}

Visualizer::Visualizer(){
}

Visualizer::~Visualizer(){
    cv::destroyWindow("Visualizer");  
}

int Visualizer::update_bg_frame(){
    
    if (videoframe.empty()) 
    {
        std::cout << "\n Image not created. You have done something wrong. \n";
        return 0;    // Unsuccessful.
    }

    if (update_counter%update_ratio==0){                                    // is this a legitimate solution? otherwise try threads
        cv::imshow("Visualizer", videoframe);//Showing the video//
        cv::waitKey(1); //Allowing 1 milliseconds frame processing time
        change_BG_color();
    }
    update_counter++;
    return 1;
}

int Visualizer::stream_frames(float *in){
    if(!update_bg_frame()){
        std::cout<<"Visualizer::stream_frames : error when showing backround"<<std::endl;
    }

    // audio_visualizer(); #
    // user_visualizer();  #
}


void Visualizer::change_BG_color(){

    if (ascR){
        if (R>=MAX) {
            ascR=false;
            R-=incrR;
        }else R+=incrR;

    }else{
        if(R<=MIN){
            ascR=true;
            R+=incrR;
        }else R-=incrR;
    }

    if (ascG){
        if (G>=MAX) {
            ascG=false;
            G-=incrG;
        }else G+=incrG;

    }else{
        if(G<=MIN){
            ascG=true;
            G+=incrG;
        }else G-=incrG;
    }
    
    if (ascB){
        if (B>=MAX) {
            ascB=false;
            B-=incrB;
        }else B+=incrB;

    }else{
        if(B<=MIN){
            ascB=true;
            B+=incrB;
        }else B-=incrB;
    }
    // std::cout<<"RGB2:"<<B<<" "<<R<<" "<<G<<std::endl;
    // std::cout<<incrR<<" "<<incrG<<" "<<incrB<<std::endl;
    // std::cout<<ascR<<" "<<ascG<<" "<<ascB<<std::endl;
    cv::Scalar color(B,R,G);
    videoframe.setTo(color);
}
