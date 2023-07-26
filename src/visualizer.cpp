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

int Visualizer::empty_visualizer(){
    
    if (videoframe.empty()) 
    {
        std::cout << "\n Image not created. You have done something wrong. \n";
        return -1;    // Unsuccessful.
    }

    if (update_counter%update_ratio==0){
        cv::imshow("Visualizer", videoframe);//Showing the video//
        cv::waitKey(1); //Allowing 1 milliseconds frame processing time

        change_colors();

        cv::Scalar color(B,R,G);
        videoframe.setTo(color);
    }
    update_counter++;
    return 0;
}

int Visualizer::stream_frames(float *in){
    int success=empty_visualizer();

    // audio_visualizer(); #
    // user_visualizer();  #
}


void Visualizer::change_colors(){
    std::cout<<"RGB1:"<<B<<" "<<R<<" "<<G<<std::endl;
    std::cout<<incrR<<" "<<incrG<<" "<<incrB<<std::endl;
    std::cout<<ascR<<" "<<ascG<<" "<<ascB<<std::endl;

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
    std::cout<<"RGB2:"<<B<<" "<<R<<" "<<G<<std::endl;
    std::cout<<incrR<<" "<<incrG<<" "<<incrB<<std::endl;
    std::cout<<ascR<<" "<<ascG<<" "<<ascB<<std::endl;
}
