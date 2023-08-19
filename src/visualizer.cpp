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
    
    dft=new double[H];
    
    std::deque<double> wave(width*redxtrans);
    x_trans=0;
    ascX=true;
    redxtrans=1;

    f_x_trans=0; // the x transition for the spectrogram

    fps=30;
    bufferCount=0;
    buffer_size=bufferSize;
    SR=sampleRate;
    
    buffersPerFrame=(SR/buffer_size)/fps;
    std::cout<<"buffersPerFrame "<<buffersPerFrame<<std::endl;

    sp=new Spetrogram(buffer_size,buffersPerFrame,H);
}

Visualizer::Visualizer(){
    
}

Visualizer::~Visualizer(){
    
    
    cv::destroyWindow("Visualizer");  
    videoframe.release();
    sp->~Spetrogram();  
    delete[] dft;
}

void Visualizer::showFrame(){
    cv::imshow("Visualizer", videoframe);//Showing the video//
    cv::waitKey(1); //Allowing 1 milliseconds frame processing time
}

int Visualizer::stream_frames(double* in,bool isBeat){
    // std::cout<<"bufferCount -->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<bufferCount<<" ";
    buffer=in;

    if (bufferCount==0){                                    // is this a legitimate solution? otherwise try threads

        // std::cout<<" shows the frame"<<std::endl;

        showFrame();

        //check white pixels
        // int white_pixel_counter=0;
        // for (int i=0;i<W;i++){
        //     for (int j=H/5;j<(H-H/5);j++){
        //         if(videoframe.at<cv::Vec3b>(j,i)[0]==255) white_pixel_counter++;
        //     }
        // }
//debug ==
        // std::cout<<white_pixel_counter<<" == "<<wave.size()<<" ??"<<std::endl;
        // at the end
        if(!update_BG_frame()){
            std::cout<<"Visualizer::stream_frames : error update_bg_frame"<<std::endl;
        }
    }else {
        // std::cout<<" is processing the frame"<<std::endl;
    }

    //start preparing for the next frame
    sp->prepare_spectrogram(bufferCount,in);


    if (isBeat){
        if (incrR<17) incrR+=7;
        int tincrR=incrR;
        int tincrG=incrG;
        int tincrB=incrB;

        if (incrR<17) incrR+=7;
        else incrR=3;
        if (incrG>5) incrG+=1;
        else incrG=1;
        if (incrB>21) incrB+=3; 
        else incrB=7;

        if(!update_BG_frame()){
            std::cout<<"Visualizer::stream_frames : error update_bg_frame"<<std::endl;
        }
    
        incrR=tincrR;
        incrG=tincrG;
        incrB=tincrB;   
    }
    
    update_wave_frame();

    
    if (bufferCount==buffersPerFrame-1){ // last frame to process before showing 
    //do something special using the last buffer?? --> compute the FFT for the concatenated signal
        // std::cout<<"computes the FFT"<<std::endl;
        
        update_spectrogram(in);
    }

    bufferCount++;
    bufferCount%=buffersPerFrame;

         
    return 1;
}

int Visualizer::update_BG_frame(){
    if (videoframe.empty()) 
    {
        std::cout << "\n Image not created. You have done something wrong. \n";
        return 0;    // Unsuccessful.
    }
    // change_BG_color();
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


int Visualizer::update_spectrogram(double *in){
    // dft=
    sp->computeFFT(dft);
    for (int i=0;i<H;i++){
        int f_y_trans=i;
        // std::cout<<"videoframe[y:"<<f_y_trans<<"][x:"<<f_x_trans<<"]="<<dft[i]<<"-------------->*255="<<(int)(dft[i]*255)<<" made "<<(int)(dft[i]*255)%255<<std::endl;
        videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[0] = (int)(dft[i]*255)%255;//newval[0];
        videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[1] = (int)(dft[i]*255)%255;//newval[1];
        videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[2] = (int)(dft[i]*255)%255;//newval[2];
    }
    f_x_trans++;
    f_x_trans%=W;
    
//append to dft
    // if(bufferCount%buffersPerFrame==1)
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
    // int numBuffer=bufferCount%buffersPerFrame;
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