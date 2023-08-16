#include "visualizer.h"

// void* Visualizer::visualize(void* args){
//     static_cast<Visualizer*>(args)->stream_frames();
//     return NULL;   
//     // return ((Visualizer *)args)->stream_frames(in);
// }

Visualizer::Visualizer(int width,int height,int sampleRate,int bufferSize){
    W=width;
    H=height;
    cv::namedWindow("Visualizer",cv::WINDOW_AUTOSIZE);
    cv::Mat img(height,width, CV_8UC3,cv::Scalar(0,0,0));
    videoframe = img;

    R=0;
    G=0;
    B=0;
    
    incrR=3;
    incrG=1;
    incrB=7;
    ascR=true;
    ascG=true;
    ascB=true;
    
    dft=NULL;
    
    std::deque<float> wave(width*redxtrans);
    x_trans=0;
    ascX=true;
    redxtrans=1;

    update_counter=0;
    buffer_size=bufferSize;
    SR=sampleRate;

    update_ratio=5;
}

Visualizer::Visualizer(){
}

Visualizer::~Visualizer(){
    cv::destroyWindow("Visualizer");  
}

int Visualizer::stream_frames(float* in,bool isBeat){
    buffer=in;

    if (update_counter%update_ratio==0){                                    // is this a legitimate solution? otherwise try threads

        cv::imshow("Visualizer", videoframe);//Showing the video//
        cv::waitKey(1); //Allowing 1 milliseconds frame processing time

        //check white pixels
        int white_pixel_counter=0;
        for (int i=0;i<W;i++){
            for (int j=H/5;j<(H-H/5);j++){
                if(videoframe.at<cv::Vec3b>(j,i)[0]==255) white_pixel_counter++;
            }
        }
//debug ==
        // std::cout<<white_pixel_counter<<" == "<<wave.size()<<" ??"<<std::endl;
        // at the end

        
        update_counter%=update_ratio;
}

    if (isBeat){
        if (incrR<17) incrR+=7; 
        else incrR=3;
        if (incrG>5) incrG+=1; 
        else incrG=1;
        if (incrB>21) incrB+=3; 
        else incrB=7;

        if(!update_BG_frame()){
            std::cout<<"Visualizer::stream_frames : error update_bg_frame"<<std::endl;
        }
    }

    update_wave_frame();
    update_counter++;
    return 1;
}

int Visualizer::update_BG_frame(){
    if (videoframe.empty()) 
    {
        std::cout << "\n Image not created. You have done something wrong. \n";
        return 0;    // Unsuccessful.
    }
    change_BG_color();
    return 1;
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
    cv::Scalar color(B,R,G);
    videoframe.setTo(color);
}

int Visualizer::update_wave_frame(){
    
// KEEP ONLY A FEW
    int ctr=buffer_size; // counter to iterate over buffer
    int hop=73;// buffer stride

    while(ctr>0) {
        int idx=buffer_size-ctr;
        if ((wave.size()+buffer_size)<(W*redxtrans)){
            wave.push(buffer[idx]); //buffer[idx]
            // std::cout<<"filling sample "<<idx<<" with value "<< buffer[idx]<<" and normalized value "<<norm<<" --> wave size "<<wave.size()<<std::endl; //
            // std::cout<<" head "<<wave.front()<<" tail "<<wave.back()<<std::endl;
        }else{
            wave.pop();
            wave.push(buffer[idx]);
            // std::cout<<"updating wav with len "<<wave.size()<<" head "<<wave.front()<<" tail "<<wave.back()<<std::endl;
        }

        int y_trans=(H/2)+wave.front()*(H/2);//(wave.front()/2)+0.5*(H/2);

        if (ascX){
            if (x_trans>=W) {
                ascX=false;
                if (ctr%redxtrans==0) x_trans--;
            }else x_trans++;

        }else{
            if(x_trans<=0){
                ascX=true;
                x_trans++;
            }else {if (ctr%redxtrans==0) x_trans--;}
        }
        // std::cout<<"filling canvas with in position y "<<y_trans<<std::endl;
        // std::cout<<"filling canvas with in position x "<<x_trans<<std::endl;
        videoframe.at<cv::Vec3b>(y_trans,x_trans)[0] = 255;//newval[0];
        videoframe.at<cv::Vec3b>(y_trans,x_trans)[1] = 255;//newval[1];
        videoframe.at<cv::Vec3b>(y_trans,x_trans)[2] = 255;//newval[2];

        ctr-=hop;
    }
    return 0;
}


int Visualizer::update_spectrogram(float *fft){
//append to dft
    // if(update_counter%update_ratio==1)
    //     dft=NULL;   

    // if (!dft){
    //     dft=new float[buffer_size];
    //     for(int i=0;i<buffer_size;i++)
    //         // *(dft+i)=(fft[i]/2+0.5) * (MAX-MIN) + MIN; //fft[i]            
    //         *(dft+i)=fft[i];
            
    // }else{
    //     // realloc
    //     float* temp = new float[buffer_size];
    //     std::copy(dft, dft + buffer_size, temp); // Suggested by comments from Nick and Bojan
    //     delete [] dft;
    //     dft = temp;
    // }

// normalize fft in height
    // float* temp2=dft;
    // for (int i=0;i<buffer_size;i++){
    //     // *(temp2+i)=(fft[i]/2+0.5) * (H-0) + 0;
    //     *(temp2+i)=(fft[i]/2+0.5) * (H-0) + 0;
    // }

//visualize fft
    // int numBuffer=update_counter%update_ratio;
    // // if(numBuffer!=0){
    // int y=H/2;
    // // for (int j=0;j<buffer_size;j++){ //numBuffer*buffer_size
    // //     y=H/2 ;//fft[j];
    //     for (int x=0;x<W/5;x++){

    //         std::cout<<"x "<<x<<" y "<<y<<std::endl;
    //         videoframe.at<cv::Vec3b>(y,x)[0] = 0;//newval[0];
    //         videoframe.at<cv::Vec3b>(y,x)[1] = 255;//newval[1];
    //         videoframe.at<cv::Vec3b>(y,x)[2] = 0;//newval[2];
    //     }

    
    return 1;
    // std::cout<<"AFTER:dft[0]:"<<dft[0]<<" &dft:"<<&dft<<std::endl;
}