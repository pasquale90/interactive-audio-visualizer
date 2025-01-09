#include "visualizer.h"
#include <cstddef>

constexpr int qASCII {113}; // 113 q
// constexpr int spaceASCII {32};// 32 space

Visualizer::Visualizer(){

    int W=cfg.dispconf.dispResW.load();
    int H=cfg.dispconf.dispResH.load();

    cv::namedWindow("Interactive Audio Visualizer",cv::WINDOW_AUTOSIZE);
    cv::Mat img( H , W, CV_8UC3,cv::Scalar(0,0,0));
    visualFrame = img;
       
    // _create_camMask();

    int cameraW = cfg.camconf.camResW.load();
    int cameraH = cfg.camconf.camResH.load();
    LR = W - cameraW;
    TB = H - cameraH;

    transpose_ratio_x = static_cast<float>(W) / static_cast<float>(cameraW) / 2.0f;
    transpose_ratio_y = static_cast<float>(H) / static_cast<float>(cameraH) / 2.0f;

trackingToggle = false;
    std::cout<<"Visualizer constructed"<<std::endl;

}

void Visualizer::setAudiolizerUpdater(std::function<void(const bool, const bool, const RegionOfInterest&, int&)> function){
    updateAudioLizer = std::move(function);
}

void Visualizer::updateTrackingMode(bool trackingEnabled){

    if (trackingToggle!=trackingEnabled){

        std::cout<<"\t\tupdateTrackingMode"<<std::endl;

        if (!trackingToggle && trackingEnabled){
            videoTracker.initializeTracker(cameraFrame);
            std::cout<<"\n \t\t\tinitializing tracker \n"<<std::endl;
        }
        trackingToggle = trackingEnabled;
        
    }

}

void Visualizer::broadcast(){

    bool trackingEnabled,trackingUpdated;
    RegionOfInterest trackingSig;
    int frequency;

    while(true){

        // camera in visualizer
        bool frameElapsed = camera.capture(cameraFrame); // get data
        if (!frameElapsed){
            return;
        }   

        float remaining_percentage;
        trackingEnabled = trigger.isTrackingEnabled(remaining_percentage);
        updateTrackingMode(trackingEnabled);

        if (trackingEnabled){ // preprocess visual_frame -->   doesn't depict the frame, it just edits it so it does not require a new frame to be captured by the camera.
            
            trackingUpdated = videoTracker.trackObject(cameraFrame, trackingSig);

            if (!trackingUpdated){
                // reset
                trigger.reset();
            }            
                
            // update the current visualframe according to the changing of the tracking stimulus
            // _set_BG_manually(frequency, trackingEnabled);
            // update_spectrogram();
            _set_FG_manually(trackingSig);


        }else{
            _setToCamera(remaining_percentage);
            trackingUpdated = trackingEnabled = false;
        }

        updateAudioLizer(trackingUpdated, trackingEnabled, trackingSig, frequency);

        bool exit_msg = _showFrame();
        if (exit_msg)
            break;
    }
}

Visualizer::~Visualizer(){
    cv::destroyWindow("Interactive Audio Visualizer");  
    visualFrame.release();
    cameraFrame.release();
    // camBinaryMask.release();
    std::cout<<"Visualizer destructed"<<std::endl;
}

/*
void Visualizer::_create_camMask(){

    int cameraW = cfg.camconf.camResW.load();
    int cameraH = cfg.camconf.camResH.load();
    int W=cfg.dispconf.dispResW.load();
    int H=cfg.dispconf.dispResH.load();

    // calculate areas
    int r = (cameraW>cameraH) ? cameraH/2 : cameraW/2;
    
    // int outArea = pow( 2*r, 2 ) - (M_PI * pow(r,2)); // pow( 2*r, 2 ) is the area of the box (same center, 2*r both edges) which is subtracted by the circle area πr^2
    // outArea+= (abs(cameraW-cameraH) * r); // abs(cameraW-cameraH) = rest area outside the camera frame

    int center_x = cameraW/2;
    int center_y = cameraH/2;

    numPointsPerimeter = floor((sqrt(2)*(r-1)+4)/2)*8;  // https://stackoverflow.com/a/14995443/15842840

    cv::Mat m1 = cv::Mat(cameraH,cameraW, CV_64F, cv::Scalar(0)); // CV_32F
    camBinaryMask=m1;

r = cfg.iavconf.roiRadius;
    int thickness=1;
    circle( camBinaryMask,
        cv::Point(center_x,center_y),
        r,
        cv::Scalar( 0, 255, 0 ),
        thickness=1,
        cv::LINE_8);
    
    for (int i=0;i<cameraW;i++){
        for (int j=0;j<cameraH;j++){
            
            double center_dist = (double) sqrt ( pow((i-center_x),2) + pow((j-center_y),2) );
            
            // https://stackoverflow.com/a/839931/15842840
            // Get the max distance for each point to normalize the distance between square and circles perimeter
            // x = cx + r * cos(a)
            // y = cy + r * sin(a)
            double max_dist = (double) sqrt ( pow((i-center_x),2) + pow((j-center_y),2) );
            bool condition = center_dist > (double)r ;
            
            //(x - a)**2 + (y - b)**2 == r**2;
            // double term1 = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2));
            // double term2 = pow(r,2);
            // bool condition2 = term1 >= term2;
            // bool condition2 = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2)) >= 

            if (condition){
                // double transparency = (center_dist-(double)r)/ (double)abs(cameraW-cameraH);
                double transparency = (center_dist-(double)r)/ (double)sqrt(pow(cameraW-cameraH,2));
            
                camBinaryMask.at<double>(j,i) = transparency;

                if (max_dist == r+4 || center_dist == r+3 || center_dist == r+1 || center_dist == r+2 || center_dist == r){
                    int T = (H - cameraH)/2;
                    int L = (W - cameraW)/2;
                    int x = L + i;
                    int y = T + j;
                    visualFrame.at<cv::Vec3b>(y,x)[0] = 137;
                    visualFrame.at<cv::Vec3b>(y,x)[1] = 137;
                    visualFrame.at<cv::Vec3b>(y,x)[2] = 137;
                }
            }
        }
    }
}
*/

bool Visualizer::_showFrame(){ 
    cv::imshow("Interactive Audio Visualizer", visualFrame);
    int msg = cv::waitKey(1);
    if (msg == qASCII) return true; 
    // else if (cfg.iavconf.trigger=="Manual" && msg == spaceASCII)    
    return false;
}

#include "unused_defines.h"
void Visualizer::_set_BG_manually(int tone, bool UNUSED(trackEnabled)){

    float percent;

// naive conversion
    // if (tone>0 && tone<200){                  
    //     percent = (double)tone/200.;
    //     for (int i=0;i<W;i++)
    //         for (int j=0;j<H;j++){
    //             // std::cout<<"visualFrame.at<cv::Vec3b>(j,i)[0] = (int)(255.*percent) "<<(int)(255.*percent)<<std::endl;
    //             visualFrame.at<cv::Vec3b>(j,i)[0] = (int)(255.*percent);
    //         }
    // }else if (tone >200 && tone <600){
    //     percent = (double)tone/(600. - 200.);
    //     for (int i=0;i<W;i++)
    //         for (int j=0;j<H;j++){
    //             // std::cout<<"visualFrame.at<cv::Vec3b>(j,i)[0] = (int)(255.*percent) "<<(int)(255.*percent)<<std::endl;
    //             visualFrame.at<cv::Vec3b>(j,i)[1] = (int)(255.*percent);
    //         }
    // }else if (tone >600 && tone <4000) {
    //     percent = (double)tone/(4000. - 600.);
    //     for (int i=0;i<W;i++)
    //         for (int j=0;j<H;j++){
    //             // std::cout<<"visualFrame.at<cv::Vec3b>(j,i)[0] = (int)(255.*percent) "<<(int)(255.*percent)<<std::endl;
    //             visualFrame.at<cv::Vec3b>(j,i)[3] = (int)(255.*percent);
    //         }
    // }
    // std::cout<<"Visualizer percent "<<percent<<std::endl;

// works but boring 
        // int B = visualFrame.at<cv::Vec3b>(0,0)[0];
        // int G = visualFrame.at<cv::Vec3b>(0,0)[1];
        // int R = visualFrame.at<cv::Vec3b>(0,0)[2];
        // if (tone>0 && tone<200){                // keep blue             
        //     B = 255;
        //     percent = (double)tone/600.;    // low trans
        //     G = (int)(255.*percent);
        //     percent = (double)tone/200.;  // high trans
        //     R = (int)(255.*percent);
        // }
        // else if (tone >200 && tone <600){       // keep green
        //     percent = (double)tone/(600. - 200.); // low trans
        //     B = (int)(255.*percent);
        //     G = 255;
        //     percent = (double)tone/(600. - 200.);
        //     R = (int)(255.*percent);
        // }
        // else if (tone >600 && tone <4000) {
        //     percent = (double)tone/(4000. - 600.);
        //     B = (int)(255.*percent);
        //     percent = (double)tone/(4000. - 600.);
        //     G = (int)(255.*percent);
        //     R = 255;
        // }
        // visualFrame.setTo( ( B, G, R ) );
        // std::cout<<"Visualizer percent "<<percent<<std::endl;

        // for using a percent amount for chaning color
        // int B = visualFrame.at<cv::Vec3b>(0,0)[0];
        // int G = visualFrame.at<cv::Vec3b>(0,0)[1];
        // int R = visualFrame.at<cv::Vec3b>(0,0)[2];

        int B {0},G {0},R {0};
        if (tone> cfg.iavconf.minFrequency && tone<=300){                // keep blue             
            percent = static_cast<float>(tone)/300.0f;  // high trans
            B = 255;
            G = static_cast<int>(255.*percent);
            R = static_cast<int>(255.*(1.-percent));
        }
        else if (tone >300 && tone <=700){       // keep green
            percent = static_cast<float>(tone)/700.0f; // low trans
            B = static_cast<int>(255.*percent);
            G = 255;
            R = static_cast<int>(255.*(1.-percent));
        }
        else if (tone >700 && tone <= cfg.iavconf.maxFrequency) {    // keep red
            percent = static_cast<float>(tone)/static_cast<float>(cfg.iavconf.maxFrequency);            
            B = static_cast<int>(255.*percent);
            G = static_cast<int>(255.*(1.-percent));
            R = 255;
        }
        visualFrame.setTo( cv::Scalar( B, G, R ) );
}

void Visualizer::drawSmallcircle(const RegionOfInterest &roi){

    int roicenterX = roi.centerX.load();
    int roicenterY = roi.centerY.load();
    int roiVolumeW = roi.volumeW.load();
    int roiVolumeH = roi.volumeH.load();

    int W=cfg.dispconf.dispResW.load();
    int H=cfg.dispconf.dispResH.load();
    
    int center_x = LR/2 + roicenterX; //  
    int center_y = TB/2 + roicenterY; // H/2;

    if ( center_x < W/2 ) //-W*2/3)
        center_x -= static_cast<int>(transpose_ratio_x * (static_cast<float>(W) / 2.0f - static_cast<float>(center_x)));
    else if ( center_x > W/2) //*2/3)
        center_x += static_cast<int>(transpose_ratio_x * (static_cast<float>(center_x) - static_cast<float>(W) / 2.0f));
    if ( center_y < H/2 )
        center_y -= static_cast<int>(transpose_ratio_y * (static_cast<float>(H) / 2.0f - static_cast<float>(center_y)));
    else if ( center_y > H/2 )
        center_y += static_cast<int>(transpose_ratio_y * (static_cast<float>(center_y) - static_cast<float>(H) / 2.0f));

    cv::Point center(center_x, center_y);//Declaring the center point
    int radius = roiVolumeW > roiVolumeH ? roiVolumeW/2 : roiVolumeH/2; //Declaring the radius
    cv::Scalar line_Color(0, 0, 0);//Color of the circle
    int thickness = 2;//thickens of the line
    circle(visualFrame, center,radius, line_Color, thickness);//Using circle()functi

}

void Visualizer::draWaveform(){
    
}

void Visualizer::_set_FG_manually(const RegionOfInterest &roi){

    drawSmallcircle(roi);
    draWaveform();
}

void Visualizer::_show_timer(float percent) {
    
    int x = cameraFrame.cols / 2;
    int y = cameraFrame.rows / 2;

    int radius = cfg.iavconf.roiRadius;  
    int thickness = 3;
    float angle = percent * 360.0f;
    cv::circle(cameraFrame, cv::Point(x, y), radius, CV_RGB(245, 245, 245), thickness);

    // int r = 255 - (millisecondsElapsed * 5) % 255;  // Gradually change the color to orange/yellow
    // int g = (millisecondsElapsed * 2) % 255;
    // int b = (millisecondsElapsed * 4) % 255;
    int r = static_cast<int>(127.5 * (1 + sin(angle * M_PI / 180.0)));  // Sinusoidal for red
    int g = static_cast<int>(127.5 * (1 + sin((angle + 120) * M_PI / 180.0)));  // Sinusoidal for green
    int b = static_cast<int>(127.5 * (1 + sin((angle + 240) * M_PI / 180.0)));  // Sinusoidal for blue

    cv::ellipse(cameraFrame, cv::Point(x, y), cv::Size(radius, radius), 0, -90, -90 + angle, CV_RGB(r, g, b), thickness);

}

void Visualizer::_setToCamera(float remaining_percentage){

    _show_timer(remaining_percentage);

    int cameraW=cameraFrame.cols;
    int cameraH=cameraFrame.rows;
    // top left of the visualFrame is ...
    int L = (visualFrame.cols - cameraW)/2;
    int T = (visualFrame.rows - cameraH)/2;

    // int r = (cameraW>cameraH) ? cameraH/2 : cameraW/2;

// @ THIS IS POSTPONED -- > RELATED TO CAMBINARYMASK METHOD 
// @ ALSO, IS THIS THE METHOD TO DEPICT THE ROI? 

    // // draw transparent pixels in a form of enclosed circle within camera frame
    // double vB = (double)visualFrame.at<cv::Vec3b>(0,0)[0];
    // double vG = (double)visualFrame.at<cv::Vec3b>(0,0)[1];
    // double vR = (double)visualFrame.at<cv::Vec3b>(0,0)[2];
    // for (int i=0;i<cameraW;i++){
    //     for (int j=0;j<cameraH;j++){
    //         if (camBinaryMask.at<double>(j,i)>0.){
    //             double ratio = camBinaryMask.at<double>(j,i);
    //             cameraFrame.at<cv::Vec3b>(j,i)[0] = ((ratio*vB) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[0])/2.;
    //             cameraFrame.at<cv::Vec3b>(j,i)[1] = ((ratio*vG) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[1])/2.;
    //             cameraFrame.at<cv::Vec3b>(j,i)[2] = ((ratio*vR) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[2])/2.;
    //         }
    //     }
    // }
    cameraFrame.copyTo(visualFrame(cv::Rect(L,T,cameraFrame.cols, cameraFrame.rows)));
}



/*
int Visualizer::and_Sound_into_Image(float* left, float* right,cv::Mat videoframe, bool frameElapsed, bool trackEnabled, int tone,RegionOfInterest roi){
    
    bool exit_msg=false;

    if (trackEnabled) {  // <-- REMOVE IF YOU WAN T TO PROCESS BG MUSIC - apart from sine waves
        wf.prepare_waveform(left,right); //--> addressed to the next update
        // sp.prepare_spectrogram((double*)left,(double*)right);
    }
    if (frameElapsed){ // checks if new data available.
        if (trackEnabled){ // preprocess visual_frame -->   doesn't depict the frame, it just edits it so it does not require a new frame to be captured by the camera.
            // update the current visualframe according to the changing of the tracking stimulus
            _set_BG_manually(tone, trackEnabled);
            // update_spectrogram();
            _set_FG_manually(videoframe,roi);
        }else{
            _setToCamera(videoframe);
        }
        exit_msg = _showFrame();
    }
    return exit_msg;
}
*/

// int Visualizer::update_spectrogram(){
//     /***
//      * Old structured kept. Requires new implementation...
//      * Code below is provided as an example for accessing fft data
//      */ 
    // double minf,maxf;
    // sp.computeFFT(dft,minf,maxf);
    // for (int i=0;i<H;i++){
    //     int f_y_trans=i;
        //normalize min max in range [0,1]
        // dft[i]=(dft[i]-minf)/(maxf-minf);
        // std::cout<<"visualFrame[y:"<<f_y_trans<<"][x:"<<f_x_trans<<"]="<<dft[i]<<"-------------->*255="<<(int)(dft[i]*255)<<" made "<<(int)(dft[i]*255)%255<<std::endl;
        // visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[0] = (int)(dft[i]*255)%255;//newval[0]; *0.7
        // visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[1] = (int)(dft[i]*255)%255;//newval[1]; *0.3
        // visualFrame.at<cv::Vec3b>(f_y_trans,f_x_trans)[2] = (int)(dft[i]*255)%255;//newval[2]; *0.2
//     }
//     return 1;
// }

/* efficient but does not iterate in an ordered sequence over all perimeter pixels - probably requires mapping from midpoint-out to waveform i and j coordinates
// https://www.geeksforgeeks.org/mid-point-circle-drawing-algorithm/
void Visualizer::midPointCircleDraw(int x_centre, int y_centre, int r)
{
    int numPointsPerimeter;
    int x = r, y = 0;
     
    // Printing the initial point on the axes 
    // after translation
    // cout << "(" << x + x_centre << ", " << y + y_centre << ") ";
     
    // When radius is zero only a single
    // point will be printed
    if (r > 0)
    {
        // cout << "(" << x + x_centre << ", " << -y + y_centre << ") ";
        // cout << "(" << y + x_centre << ", " << x + y_centre << ") ";
        // cout << "(" << -y + x_centre << ", " << x + y_centre << ")\n";

        circleMask.push_back({ x + x_centre ,-y + y_centre});
        circleMask.push_back({ y + x_centre , x + y_centre });
        circleMask.push_back({-y + x_centre , x + y_centre});

        visualFrame.at<cv::Vec3b>(-y + y_centre , x + x_centre )[0] = 137;
        visualFrame.at<cv::Vec3b>( x + y_centre , y + x_centre )[1] = 137;
        visualFrame.at<cv::Vec3b>( x + y_centre ,-y + x_centre )[2] = 137;
    }
     
    // Initialising the value of P
    int P = 1 - r;
    while (x > y)
    { 
        y++;
         
        // Mid-point is inside or on the perimeter
        if (P <= 0)
            P = P + 2*y + 1;
        // Mid-point is outside the perimeter
        else
        {
            x--;
            P = P + 2*y - 2*x + 1;
        }
         
        // All the perimeter points have already been printed
        if (x < y)
            break;
         
        // Printing the generated point and its reflection
        // in the other octants after translation
        // cout << "(" << x + x_centre << ", " << y + y_centre << ") ";
        // cout << "(" << -x + x_centre << ", " << y + y_centre << ") ";
        // cout << "(" << x + x_centre << ", " << -y + y_centre << ") ";
        // cout << "(" << -x + x_centre << ", " << -y + y_centre << ")\n";

        circleMask.push_back({ x + x_centre , y + y_centre});
        circleMask.push_back({-x + x_centre , y + y_centre});
        circleMask.push_back({ x + x_centre ,-y + y_centre });
        circleMask.push_back({-x + x_centre ,-y + y_centre});

        visualFrame.at<cv::Vec3b>( y + y_centre , x + x_centre )[0] = 137;
        visualFrame.at<cv::Vec3b>( y + y_centre ,-x + x_centre )[1] = 137;
        visualFrame.at<cv::Vec3b>(-y + y_centre , x + x_centre )[2] = 137;
        visualFrame.at<cv::Vec3b>(-y + y_centre ,-x + x_centre )[2] = 137;

         
        // If the generated point is on the line x = y then 
        // the perimeter points have already been printed
        if (x != y)
        {
            // cout << "(" << y + x_centre << ", " << x + y_centre << ") ";
            // cout << "(" << -y + x_centre << ", " << x + y_centre << ") ";
            // cout << "(" << y + x_centre << ", " << -x + y_centre << ") ";
            // cout << "(" << -y + x_centre << ", " << -x + y_centre << ")\n";

            circleMask.push_back({ y + x_centre , x + y_centre});
            circleMask.push_back({-y + x_centre , x + y_centre});
            circleMask.push_back({ y + x_centre ,-x + y_centre });
            circleMask.push_back({-y + x_centre ,-x + y_centre});

            visualFrame.at<cv::Vec3b>( x + y_centre , y + x_centre )[0] = 137;
            visualFrame.at<cv::Vec3b>( x + y_centre ,-y + x_centre )[1] = 137;
            visualFrame.at<cv::Vec3b>(-x + y_centre , y + x_centre )[2] = 137;
            visualFrame.at<cv::Vec3b>(-x + y_centre ,-y + x_centre )[2] = 137;
        }
    }
    std::cout<<"Vector length "<< circleMask.size()<<" numPointsPerimeter "<<numPointsPerimeter<<std::endl;
}
*/
