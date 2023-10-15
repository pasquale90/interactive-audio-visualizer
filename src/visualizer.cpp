#include "visualizer.h"

// Visualizer::Visualizer(int width,int height,int sampleRate,int bufferSize, int fps) : W(width), H(height), SR(sampleRate), buffer_size(bufferSize), fps(fps){
// Visualizer::Visualizer(const Config& config):cfg(config){
Visualizer::Visualizer(const Config& cfg): 
                                        W(cfg.displayW), 
                                        H(cfg.displayH), 
                                        SR(cfg.sampleRate), 
                                        buffer_size(cfg.bufferSize), 
                                        fps(cfg.fps){

    // W=cfg.displayW;
    // H=cfg.displayH;
    // SR=cfg.sampleRate;
    // buffer_size=cfg.bufferSize;
    // fps=cfg.fps;
    
    Config conf(cfg);
    std::cout<<&cfg<<"\n"<<&conf<<std::endl;
    conf.display();
    std::cout<<"Visualizer constructor "<<W<<", "<<H<<", "<<SR<<", "<<buffer_size<<", "<<fps<<std::endl;
    // W=1024;
    // H=512;
    // SR=22050;
    // buffer_size=512;
    // fps=25;
    
    cv::namedWindow("Interactive Audio Visualizer",cv::WINDOW_AUTOSIZE);
    cv::Mat img(H,W, CV_8UC3,cv::Scalar(255,255,255));
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
    
    x_trans=0;
    ascX=true;
    redxtrans=1;

    f_x_trans=0; // the x transition for the spectrogram

    bufferCount=0;
    // buffer_size=bufferSize;
    // SR=sampleRate;
    
    dft=new double[H];
    
    buffersPerFrame=std::ceil((SR/buffer_size)/(double)fps);
    
    std::cout<<"Visualizer buffersPerFrame  "<<buffersPerFrame<<std::endl;
    wf=new Waveform(buffer_size,buffersPerFrame,W);

    sp=new Spectrogram(buffer_size,buffersPerFrame,H);
    beatCount=0;
}

Visualizer::Visualizer(){
    Config cfg();
    // requires further variables to be assigned properly ... ----------------------> DO NOT FORGET TO FIX
}

Visualizer::~Visualizer(){
    cv::destroyWindow("Visualizer");  
    videoframe.release();
    wf->~Waveform();
    sp->~Spectrogram();  
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
        // if(!update_BG_frame()){
        //     std::cout<<"Visualizer::stream_frames : error update_bg_frame"<<std::endl;
        // }
    }else {
        // std::cout<<" is processing the frame"<<std::endl;
    }

    //start preparing for the next frame
    // wf->prepare_waveform(bufferCount,in); //--> addressed to the next update
    sp->prepare_spectrogram(bufferCount,in);
    
    
    if (isBeat){
        // if (incrR<17) incrR+=7;
        // int tincrR=incrR;
        // int tincrG=incrG;
        // int tincrB=incrB;

        // if (incrR<17) incrR+=7;
        // else incrR=3;
        // if (incrG>5) incrG+=1;
        // else incrG=1;
        // if (incrB>21) incrB+=3; 
        // else incrB=7;
        std::cout<<bufferCount<<" " ;
        if(!update_BG_frame()){
            std::cout<<"Visualizer::stream_frames : error update_bg_frame"<<std::endl;
        }
        showFrame();
    
        // incrR=tincrR;
        // incrG=tincrG;
        // incrB=tincrB;   
    }
    

    if (bufferCount==buffersPerFrame-1){ // last frame to process before showing 
    //do something special using the last buffer?? --> compute the FFT for the concatenated signal
        // std::cout<<"computes the FFT"<<std::endl;
        // update_wave_frame();
        update_spectrogram();
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
    change_BG_color();
    return 1;
}

void Visualizer::change_BG_color(){
    int fR,fG,fB;

    if (ascR){
        if (R>=MAX) {
            ascR=false;
            fR=-incrR;
        }else R=+incrR;

    }else{
        if(R<=MIN){
            ascR=true;
            fR=-incrR;
        }else fR=+incrR;
    }

    if (ascG){
        if (G>=MAX) {
            ascG=false;
            fG=-incrG;
        }else fG=+incrG;

    }else{
        if(G<=MIN){
            ascG=true;
            fG=+incrG;
        }else fG=-incrG;
    }
    
    if (ascB){
        if (B>=MAX) {
            ascB=false;
            fB=-incrB;
        }else fB=+incrB;

    }else{
        if(B<=MIN){
            ascB=true;
            fB=+incrB;
        }else fB=-incrB;
    }
    cv::Scalar color(B,R,G);
    // videoframe.setTo(color);

// first way to change the color
    if (beatCount%9==0){
        for (int i=0;i<W-f_x_trans;i++){
            for (int j=0;j<H;j++){
                videoframe.at<cv::Vec3b>(j,i)[0] += fB;//newval[0];
                videoframe.at<cv::Vec3b>(j,i)[1] += fR;//newval[1];
                videoframe.at<cv::Vec3b>(j,i)[2] += fG;//newval[2];
            }
        }
    }
// second way to change the color
    else{   

        for (int i=0;i<W-f_x_trans;i++){
            for (int j=0;j<H;j++){
                if (i>H/2-H/6 && i<H/2+H/6)
                    videoframe.at<cv::Vec3b>(j,i)[0] += fB;//newval[0];
                else if (i<H/6 || i>H-H/6)
                    videoframe.at<cv::Vec3b>(j,i)[1] += fR;//newval[1];
                else
                    videoframe.at<cv::Vec3b>(j,i)[2] += fG;//newval[2];
            }
        }
    }
    beatCount++;
    beatCount%=8;
}

int Visualizer::update_wave_frame(){

    double minw,maxw;
    double *wave=wf->getWaveform(minw,maxw);;
    double Amplification=H/32;
    // int wavelen=wf->getWaveLen();
    std::cout<<"minw "<<minw<<" maxw "<<maxw<<std::endl;

    const double mean = maxw-minw;
    const double stddev = (double)H/5;
    std::default_random_engine generator;
    std::normal_distribution<double> dist(mean, stddev);

    // std::cout<<"wavelen "<<wavelen<<", width "<<W<<std::endl;
    ///*
    for (int i=1;i<buffersPerFrame;i++){
        // Define random generator with Gaussian distribution
        
        // std::cout<<"wave[i]-minw "<<wave[i]<<"-"<<minw<<"="<<wave[i]-minw<<std::endl;
        // wave[i]=(double)2.0*(wave[i]-minw)/(maxw-minw)-1;
        // int y_trans=(H/2)+wave[i]*(H/2)*2;//(wave.front()/2)+0.5*(H/2);
        // std::cout<<"Visualizer::Frame (x,y) ("<<x_trans<<","<<y_trans<<") == wave["<<i<<"] "<<wave[i]<<std::endl;

        //replacing the above code - calc x y from <buffersPerFrame> mean-waveform samples
        // double mean=wave[i];
        
        // std::cout<<"wave[i] before "<<wave[i]<<" ";
        wave[i]=(double)2.0*(wave[i]-minw)/(maxw-minw)-1;
        // std::cout<<" after "<<wave[i]<<std::endl;
        
        int y_trans=wave[i]*Amplification + dist(generator);//(wave.front()/2)+0.5*(H/2);
        std::cout<<"is added?? "<<wave[i]*Amplification<<"!="<<y_trans<<" ??"<<std::endl;
        // y_trans=abs(y_trans);

        // assert (y_trans < H/2);
        if (y_trans > H/2) y_trans = H/2-1;
        if (y_trans < 0) y_trans = 0;

        int start,end;
        if (y_trans < 0){
            start=H/2+y_trans;
            end=H/2-y_trans;
        }else{
            start=H/2-y_trans;
            end=H/2+y_trans;
        }

        std::cout<<" at x:"<<x_trans << " from H/2:"<<H/2<<" up to y: "<<y_trans<<std::endl;
        std::cout<<" start "<<start<<" end "<<end<<std::endl;
        for (int ytr = start; ytr < end ; ytr++ ){
            std::cout<<" > x,y "<<x_trans<<","<<ytr<<std::endl;
            videoframe.at<cv::Vec3b>(ytr,x_trans)[0] = 255;//newval[0];
            videoframe.at<cv::Vec3b>(ytr,x_trans)[1] = 255;//newval[1];
            videoframe.at<cv::Vec3b>(ytr,x_trans)[2] = 255;//newval[2];
        }
        x_trans++;
        if(x_trans==W) x_trans=0;
        // videoframe.at<cv::Vec3b>(y2,x_trans)[0] = 255;//newval[0];
        // videoframe.at<cv::Vec3b>(y2,x_trans)[1] = 255;//newval[1];
        // videoframe.at<cv::Vec3b>(y2,x_trans)[2] = 255;//newval[2];
        // x_trans++;
        // if(x_trans==W) x_trans=0;
    }
    //*/

/* addressed for the next update 
    // KEEP ONLY A FEW
    int ctr=buffer_size; // counter to iterate over buffer
    int hop=1;// buffer stride

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
*/
}


int Visualizer::update_spectrogram(){
    // dft=
    double minf,maxf;
    sp->computeFFT(dft,minf,maxf);
    // sp->new_approach(dft);
    for (int i=0;i<H;i++){
        int f_y_trans=i;
        //normalize min max in range [0,1]
        dft[i]=(dft[i]-minf)/(maxf-minf);

        // std::cout<<"videoframe[y:"<<f_y_trans<<"][x:"<<f_x_trans<<"]="<<dft[i]<<"-------------->*255="<<(int)(dft[i]*255)<<" made "<<(int)(dft[i]*255)%255<<std::endl;
        // videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[0] = (int)(dft[i]*255)%255;//newval[0]; *0.7
        // videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[1] = (int)(dft[i]*255)%255;//newval[1]; *0.3
        // videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[2] = (int)(dft[i]*255)%255;//newval[2]; *0.2

        if (i>H/2-H/6 && i<H/2+H/6)
            videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[0] = (int)(dft[i]*255)%255;//newval[0]; *0.7
        else if (i<H/6 || i>H-H/6)
            videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[1] = (int)(dft[i]*255)%255;//newval[1]; *0.3
        else
            videoframe.at<cv::Vec3b>(f_y_trans,f_x_trans)[2] = (int)(dft[i]*255)%255;//newval[2]; *0.2

    }
    f_x_trans++;
    f_x_trans%=W;
    return 1;
}