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
    
    // Config conf(cfg);
    // std::cout<<&cfg<<"\n"<<&conf<<std::endl;
    // conf.display();
    // std::cout<<"Visualizer constructor "<<W<<", "<<H<<", "<<SR<<", "<<buffer_size<<", "<<fps<<std::endl;
    // W=1024;
    // H=512;
    // SR=22050;
    // buffer_size=512;
    // fps=25;
    
    cv::namedWindow("Interactive Audio Visualizer",cv::WINDOW_AUTOSIZE);
    cv::Mat img(H,W, CV_8UC3,cv::Scalar(0,0,0));
    visualFrame = img;
    img.release();

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
    beatCount=0;   
}

Visualizer::~Visualizer(){
    delete[] dft;
    cv::destroyWindow("Interactive Audio Visualizer");  
    visualFrame.release();
    wf->~Waveform();
    sp->~Spectrogram();  
    std::cout<<"Visualizer destructed"<<std::endl;
}


void Visualizer::setConfig(const Config& cfg){
    // Config conf(cfg);
    // std::cout<<&cfg<<"\n"<<&conf<<std::endl;
    // conf.display();
    W=cfg.displayW;
    H=cfg.displayH;
    SR=cfg.sampleRate;
    buffer_size=cfg.bufferSize;
    fps=cfg.fps;
    std::cout<<"Visualizer constructor "<<W<<", "<<H<<", "<<SR<<", "<<buffer_size<<", "<<fps<<std::endl;
    // W=1024;
    // H=512;
    // SR=22050;
    // buffer_size=512;
    // fps=25;
    
    cv::namedWindow("Interactive Audio Visualizer",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Mask",cv::WINDOW_NORMAL);
    cv::resizeWindow("Mask",cfg.camResW,cfg.camResH);

    cv::Mat img(H,W, CV_8UC3,cv::Scalar(0,0,0));
    visualFrame = img;
    
    dft=new double[H];
    // double dft[H];
    
    buffersPerFrame=std::ceil((SR/buffer_size)/(double)fps);
    
    std::cout<<"Visualizer buffersPerFrame  "<<buffersPerFrame<<std::endl;
    wf=new Waveform(buffer_size,buffersPerFrame,W);
    // Waveform wf(buffer_size,buffersPerFrame,W);

    sp=new Spectrogram(buffer_size,buffersPerFrame,H);
    // Spectrogram sp(buffer_size,buffersPerFrame,H);
    beatCount=0;
    
    _create_camMask(cfg.camResW,cfg.camResH);
}

void Visualizer::_create_camMask(int cameraW,int cameraH){


    // int thickness=1;
    // circle( cameraFrame,
    //     cv::Point((cameraW/2),(cameraH/2)),
    //     radius,
    //     cv::Scalar( 0, 255, 0 ),
    //     thickness=1,
    //     cv::LINE_8);


    // calculate areas
    int r = (cameraW>cameraH) ? cameraH/2 : cameraW/2;
    
    int outArea = pow( 2*r, 2 ) - (M_PI * pow(r,2)); // pow( 2*r, 2 ) is the area of the box (same center, 2*r both edges) which is subtracted by the circle area πr^2
    outArea+= (abs(cameraW-cameraH) * r); // abs(cameraW-cameraH) = rest area outside the camera frame
    int center_x = cameraW/2;
    int center_y = cameraH/2;

    
    cv::Mat m1 = cv::Mat(cameraH,cameraW, CV_64F, cv::Scalar(0)); // CV_32F
    // // camBinaryMask=m1.clone();
    camBinaryMask=m1;
    // m1.copyTo(camBinaryMask);
    // m1.release();
    // cv::resize(frame, new_mg, cv::Size(new_cols, new_rows));

    int thickness=1;
    circle( camBinaryMask,
        cv::Point(center_x,center_y),
        r,
        cv::Scalar( 0, 255, 0 ),
        thickness=1,
        cv::LINE_8);
    
    
    int verifier = 0;
    for (int i=0;i<cameraW;i++){
        for (int j=0;j<cameraH;j++){
            
            float center_dist = (float) sqrt ( pow((i-center_x),2) + pow((j-center_y),2) );
            bool condition = center_dist > (float)r ;
            
            camBinaryMask.at<double>(j,i) = 0.5;

            //(x - a)**2 + (y - b)**2 == r**2;
            // bool condition2 = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2)) >= pow(r,2);

            if (condition){
                camBinaryMask.at<double>(j,i) = 1;
                ++verifier;
            }
            // if (condition2){
            //     // std::cout<<"cond for i,j "<<i<<","<<j<<" is "<<condition2<<std::endl;
            //     camBinaryMask.at<double>(j,i) = 0;
            //     m1.at<double>(j,i) = 0;
            // }
        }
    }
    // camBinaryMask=m1.clone();
    std::cout<<"outArea == verifier "<< outArea << "=="<< verifier <<" ?"<<std::endl;

// not necessary implementation ... Masking is being done on the camera frame, not in the visual frame..
    // top left of the visualFrame is ...
    // int L = (W - cameraW)/2;
    // int T = (H - cameraH)/2;
    // int r = (cameraW>cameraH) ? cameraH/2 : cameraW/2;
    // int d = 2*r;

    // https://math.stackexchange.com/a/888576/1190945 
// --> constantly results to zero
    // float num_points = 0.; //+ 4* ()
    // for (int y=0;y>r;y++){
    //     num_points += (0.5 + 0.5 * sqrt( pow(d,2) - pow((2*y-1),2) ) );
    // }
    // num_points *= 4;
    // std::cout<<"Calculated number of pixels in circle = "<<num_points<<std::endl;

//  --> cannot set the camBinaryMask to m1 --> retried but wrong results in counter 
//     std::cout<<"Before"<<std::endl;
//     // int pixel_counter;
//     // camBinaryMask = cv::Mat::zeros(1, 1, CV_64F);
//     // cv::Mat m1 = cv::Mat::zeros(cameraH,cameraW, CV_64F); // CV_32F
//     cv::Mat m1 = cv::Mat(cameraH,cameraW, CV_64F, cv::Scalar(1)); // CV_32F
//     // m1.release();
//     // camBinaryMask=m1.clone();
//     // camBinaryMask=m1;
//     m1.copyTo(camBinaryMask);
//     // m1.release();
//     // cvtColor(camBinaryMask, m1);
//     // camBinaryMask=cv::Mat::create(cameraH,cameraW, CV_64F);
//     std::cout<<"After"<<std::endl;
//     for (int i=0;i<cameraW;i++){
//         for (int j=0;j<cameraH;j++){
//             //(x - a)**2 + (y - b)**2 == r**2;
//             bool condition = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2)) >= pow(r,2);
//             // condition = true;
//             // bool condition = ((cameraW/2 + r < i) && (cameraH/2 + r < j)) || ((cameraW/2 - r > i) && (cameraH/2 - r > j));
//             if (condition){
//                 // std::cout<<"cond for i,j "<<i<<","<<j<<" is "<<condition<<std::endl;
//                 camBinaryMask.at<double>(j,i) = 0.5;
//                 m1.at<double>(j,i) = 0.5;
//                 circleMask.push_back({i,j});
//                 // std::cout<<"after cond for i,j "<<i<<","<<j<<" is "<<condition<<std::endl;
//                 // cameraFrame.at<cv::Vec3b>(j,i)[1] = 70;
//                 // cameraFrame.at<cv::Vec3b>(j,i)[2] = 70;
//                 // pixel_counter++;
//             }
//         }
//     }
// cv::imshow("Mask2", m1);//Showing the video//

// naive --> also it produces segmentatio fault
    // pixel_counter = 0;
    // for (int i=0;i<cameraW;i++){
    //     for (int j=0;j<cameraH;j++){
    //         //(x - a)**2 + (y - b)**2 == r**2;
    //         bool condition = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2)) >= pow(r,2);
    //         // condition = true;
    //         // bool condition = ((cameraW/2 + r < i) && (cameraH/2 + r < j)) || ((cameraW/2 - r > i) && (cameraH/2 - r > j));
    //         if (condition){
    //             pixel_counter++;
    //         }
    //     }
    // }
    // camBinaryMask = new int[2*pixel_counter];
    // pixel_counter=0;
    // for (int i=0;i<cameraW;i++){
    //     for (int j=0;j<cameraH;j++){
    //         //(x - a)**2 + (y - b)**2 == r**2;
    //         bool condition = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2)) >= pow(r,2);
    //         // condition = true;
    //         // bool condition = ((cameraW/2 + r < i) && (cameraH/2 + r < j)) || ((cameraW/2 - r > i) && (cameraH/2 - r > j));
    //         if (condition){
    //             // camBinaryMask++;
    //             camBinaryMask[pixel_counter]=i;
    //             pixel_counter++;
    //             camBinaryMask[pixel_counter]=j;
    //             pixel_counter++;
    //         }
    //     }
    // }
    // int num_points = 0; //+ 4* ()
    // for (int y=0;y>r;y++){
    //     num_points += (0.5 + 0.5 * sqrt( pow(d,2) - pow((2*y-1),2) ) );
    // }
    // num_points *= 4;
    // std::cout<<"Calculated number of pixels in circle = "<<num_points<<std::endl;

// old attempt
    // std::pair<int,int> *iter = pixepixelBank;
    // cv::Mat img(H,W, CV_8UC1,0);
    // camBinaryMask=img;
    // for (int i=0;i<cameraW;i++){
    //     for (int j=0;j<cameraH;j++){
    //         // (x - a)**2 + (y - b)**2 == r**2;
    //         bool isOutside = (pow((i - W/2),2) + pow((j - H/2),2)) >= pow(r,2);
    //         if (isOutside){
    //             // std::pair<int,int> xy(i,j);
    //             // iter->first=i;
    //             // iter->second=j;
    //             // std::cout<<"storing pixel "<<iter->first<<","<<iter->second<<std::endl;
    //             // iter++;          
    //         }
    //     }
    // }


// SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT
}

bool Visualizer::_showFrame(bool nativeWindow){
    // if (nativeWindow){
    //     std::cout<<"Visualizer : is VISUAL Frame"<<std::endl;
    //     cv::imshow("Interactive Audio Visualizer", visualFrame);//Showing the video//
    //     // cv::waitKey(1); //Allowing 1 milliseconds frame processing time
    //     if (cv::waitKey(1) == 113) return true;
    //     return false;
    // }
    // else {
    //     std::cout<<"Visualizer : is CAMERA Frame"<<std::endl;
    //     cv::imshow("Interactive Audio Visualizer", cameraFrame);//Showing the video//
    //     // cv::waitKey(1); //Allowing 1 milliseconds frame processing time
    //     if (cv::waitKey(1) == 113) return true;
    //     return false;

    // }

    if (nativeWindow){
        std::cout<<"Visualizer : is VISUAL Frame"<<std::endl;
    }
    else {
        std::cout<<"Visualizer : is CAMERA Frame"<<std::endl;
    }
    cv::imshow("Mask", camBinaryMask);//Showing the video//
    cv::imshow("Interactive Audio Visualizer", visualFrame);//Showing the video//
    // cv::waitKey(1); //Allowing 1 milliseconds frame processing time
    if (cv::waitKey(1) == 113) return true;
    return false;

}

void Visualizer::_set_BG_manually(int tone, bool trackEnabled){    // naive

    // #include wavelengths.h
    double percent;

    if (tone>0 && tone<200){                  // naive conversion
        percent = (double)tone/200.;
        for (int i=0;i<W;i++)
            for (int j=0;j<H;j++){
                // std::cout<<"visualFrame.at<cv::Vec3b>(j,i)[0] = (int)(255.*percent) "<<(int)(255.*percent)<<std::endl;
                visualFrame.at<cv::Vec3b>(j,i)[0] = (int)(255.*percent);
            }
    }else if (tone >200 && tone <600){
        percent = (double)tone/(600. - 200.);
        for (int i=0;i<W;i++)
            for (int j=0;j<H;j++){
                // std::cout<<"visualFrame.at<cv::Vec3b>(j,i)[0] = (int)(255.*percent) "<<(int)(255.*percent)<<std::endl;
                visualFrame.at<cv::Vec3b>(j,i)[1] = (int)(255.*percent);
            }
    }else if (tone >600 && tone <4000) {
        percent = (double)tone/(4000. - 600.);
        for (int i=0;i<W;i++)
            for (int j=0;j<H;j++){
                // std::cout<<"visualFrame.at<cv::Vec3b>(j,i)[0] = (int)(255.*percent) "<<(int)(255.*percent)<<std::endl;
                visualFrame.at<cv::Vec3b>(j,i)[3] = (int)(255.*percent);
            }
    }
    std::cout<<"Visualizer percent "<<percent<<std::endl;

}

void Visualizer::_setToCamera(cv::Mat cameraFrame){

    int cameraW=cameraFrame.cols;
    int cameraH=cameraFrame.rows;

    // top left of the visualFrame is ...
    int L = (visualFrame.cols - cameraW)/2;
    int T = (visualFrame.rows - cameraH)/2;
    
    // int radius = (cameraW>cameraH) ? cameraH/2 : cameraW/2;    
    int r = (cameraW>cameraH) ? cameraH/2 : cameraW/2;

    /***
     * produces clicks and pops
     * solution 1 : store the mask during construction and iterate over certain pixel values and relief buffer from visualizer's redundant latency
     *      requires pixel calculation of circle || area outside circle --> a binary mask
     * solution 2 : run in separate thread
     */
    
    // for (int i=0;i<cameraW;i++){
    //     for (int j=0;j<cameraH;j++){

            
    //         // if (center_x + radius < f_endX) and (center_y + radius < f_endY) and (center_x - radius > f_startX) and (center_y - radius >  f_startY)
    //         // bool cond2 = (cameraW/2 + r < i) && (cameraH/2 + r < j) && (cameraW/2 - r > i) && (cameraH/2 - r > j);

    //         //(x - a)**2 + (y - b)**2 == r**2;
    //         bool condition = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2)) >= pow(r,2);
    //         // bool condition = ((cameraW/2 + r < i) && (cameraH/2 + r < j)) || ((ameraW/2 - r > i) && (cameraH/2 - r > j));
    //         if (condition){
    //             // std::cout<<"cond for i,j "<<i<<","<<j<<" is "<<cond2<<std::endl;
    //             cameraFrame.at<cv::Vec3b>(j,i)[0] = 70;
    //             cameraFrame.at<cv::Vec3b>(j,i)[1] = 70;
    //             cameraFrame.at<cv::Vec3b>(j,i)[2] = 70;
    //         }
    //     }
    // }

    std::cout<<"circleMask "<<circleMask.size()<<std::endl;

    // for (auto m : circleMask){
    //     // cameraFrame.at<cv::Vec3b>(m.second,m.first)[0] = 70;
    //     // cameraFrame.at<cv::Vec3b>(m.second,m.first)[1] = 70;
    //     // cameraFrame.at<cv::Vec3b>(m.second,m.first)[1] = 70;
    //     std::cout<<"circleMask "<<m.first<<","<<m.second<<std::endl;
    // }
    // cv::Rect rect(0,0,cameraFrame.cols, cameraFrame.rows);
    // cv::Mat extractedImage2 = videoframe(rect);
    // cameraFrame.copyTo(videoframe(rect1));
    std::cout<<"hello there"<<std::endl;
    cameraFrame.copyTo(visualFrame(cv::Rect(L,T,cameraFrame.cols, cameraFrame.rows)));


}

int Visualizer::and_Sound_into_Image(double* in,cv::Mat videoframe, bool frameElapsed, bool trackEnabled, int tone){
    
    bool exit_msg=false;

    // exit_msg = showFrame();

    sp->prepare_spectrogram(bufferCount,in);
    
    if (frameElapsed){                                    // is this a legitimate solution? otherwise try threads
        // videoframe.copyTo(cameraFrame);
        // cameraFrame.copyTo(videoframe);

        _setToCamera(videoframe);
        exit_msg = _showFrame(true);   // shows the video captured from camera --> where should it depict the visualFrame?

    }
    
    if (trackEnabled){ // preprocess visual_frame -->   does nt depict the frame, it just edits it so it does not require a new frame to be captured by the camera.
        // update the current visualframe according to the changing of the tracking stimulus
        _set_BG_manually(tone, trackEnabled);
    }


    //start preparing for the next frame
    // wf->prepare_waveform(bufferCount,in); //--> addressed to the next update

    /*
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
    */

    bufferCount++;
    bufferCount%=buffersPerFrame;

         
    return exit_msg;
}

int Visualizer::stream_frames(double* in,bool isBeat){
    // std::cout<<"bufferCount -->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<bufferCount<<" ";
    buffer=in;
    bool exit_msg=false;

    if (bufferCount==0){                                    // is this a legitimate solution? otherwise try threads

        // std::cout<<" shows the frame"<<std::endl;

        exit_msg = _showFrame(true); 

        //check white pixels
        // int white_pixel_counter=0;
        // for (int i=0;i<W;i++){
        //     for (int j=H/5;j<(H-H/5);j++){
        //         if(visualFrame.at<cv::Vec3b>(j,i)[0]==255) white_pixel_counter++;
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

    /*
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
    */

    bufferCount++;
    bufferCount%=buffersPerFrame;

         
    return exit_msg;
}

int Visualizer::update_BG_frame(){
    if (visualFrame.empty()) 
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
    // visualFrame.setTo(color);

// first way to change the color
    if (beatCount%9==0){
        for (int i=0;i<W-f_x_trans;i++){
            for (int j=0;j<H;j++){
                visualFrame.at<cv::Vec3b>(j,i)[0] += fB;//newval[0];
                visualFrame.at<cv::Vec3b>(j,i)[1] += fR;//newval[1];
                visualFrame.at<cv::Vec3b>(j,i)[2] += fG;//newval[2];
            }
        }
    }
// second way to change the color
    else{   

        for (int i=0;i<W-f_x_trans;i++){
            for (int j=0;j<H;j++){
                if (i>H/2-H/6 && i<H/2+H/6)
                    visualFrame.at<cv::Vec3b>(j,i)[0] += fB;//newval[0];
                else if (i<H/6 || i>H-H/6)
                    visualFrame.at<cv::Vec3b>(j,i)[1] += fR;//newval[1];
                else
                    visualFrame.at<cv::Vec3b>(j,i)[2] += fG;//newval[2];
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
            visualFrame.at<cv::Vec3b>(ytr,x_trans)[0] = 255;//newval[0];
            visualFrame.at<cv::Vec3b>(ytr,x_trans)[1] = 255;//newval[1];
            visualFrame.at<cv::Vec3b>(ytr,x_trans)[2] = 255;//newval[2];
        }
        x_trans++;
        if(x_trans==W) x_trans=0;
        // visualFrame.at<cv::Vec3b>(y2,x_trans)[0] = 255;//newval[0];
        // visualFrame.at<cv::Vec3b>(y2,x_trans)[1] = 255;//newval[1];
        // visualFrame.at<cv::Vec3b>(y2,x_trans)[2] = 255;//newval[2];
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
        visualFrame.at<cv::Vec3b>(y_trans,x_trans)[0] = 255;//newval[0];
        visualFrame.at<cv::Vec3b>(y_trans,x_trans)[1] = 255;//newval[1];
        visualFrame.at<cv::Vec3b>(y_trans,x_trans)[2] = 255;//newval[2];

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

        // std::cout<<"visualFrame[y:"<<f_y_trans<<"][x:"<<f_x_trans<<"]="<<dft[i]<<"-------------->*255="<<(int)(dft[i]*255)<<" made "<<(int)(dft[i]*255)%255<<std::endl;
        // visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[0] = (int)(dft[i]*255)%255;//newval[0]; *0.7
        // visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[1] = (int)(dft[i]*255)%255;//newval[1]; *0.3
        // visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[2] = (int)(dft[i]*255)%255;//newval[2]; *0.2

        if (i>H/2-H/6 && i<H/2+H/6)
            visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[0] = (int)(dft[i]*255)%255;//newval[0]; *0.7
        else if (i<H/6 || i>H-H/6)
            visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[1] = (int)(dft[i]*255)%255;//newval[1]; *0.3
        else
            visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[2] = (int)(dft[i]*255)%255;//newval[2]; *0.2

    }
    f_x_trans++;
    f_x_trans%=W;
    return 1;
}