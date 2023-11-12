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
    
    // wf=new Waveform(buffer_size,cfg.radius,W);
    // sp=new Spectrogram(buffer_size,buffersPerFrame,H);
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
    // cv::destroyWindow("Mask");  
    visualFrame.release();
    camBinaryMask.release();
    wf.~Waveform();
    sp.~Spectrogram();  
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

    fmin = cfg.minFrequency;
    fmax = cfg.maxFrequency;
    std::cout<<"Visualizer constructor "<<W<<", "<<H<<", "<<SR<<", "<<buffer_size<<", "<<fps<<std::endl;
    // W=1024;
    // H=512;
    // SR=22050;
    // buffer_size=512;
    // fps=25;
    
    cv::namedWindow("Interactive Audio Visualizer",cv::WINDOW_AUTOSIZE);
    // cv::namedWindow("Mask",cv::WINDOW_NORMAL);
    // cv::resizeWindow("Mask",cfg.camResW,cfg.camResH);

    cv::Mat img(H,W, CV_8UC3,cv::Scalar(0,0,0));
    visualFrame = img;
    
    dft=new double[H];
    
    buffersPerFrame=std::ceil((SR/buffer_size)/(double)fps);
    
    // wf=new Waveform(buffer_size,buffersPerFrame,W);
    wf.set_config(cfg);
    // Waveform wf(buffer_size,buffersPerFrame,W);
    // sp=new Spectrogram(buffer_size,buffersPerFrame,H);
    // sp.set_config(cfg);

    // Spectrogram sp(buffer_size,buffersPerFrame,H);

    beatCount=0;
    
    _create_camMask(cfg.camResW,cfg.camResH);

    std::cout<<"Visualizer constructed"<<std::endl;

}
/* efficient but does not iterate in an ordered sequence over all perimeter pixels - probably requires mapping from midpoint-out to waveform i and j coordinates
// https://www.geeksforgeeks.org/mid-point-circle-drawing-algorithm/
void Visualizer::midPointCircleDraw(int x_centre, int y_centre, int r)
{
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
    std::cout<<"Vector length "<< circleMask.size()<<" numberofpixels "<<numberofpixels<<std::endl;
}
*/

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

    numberofpixels = floor((sqrt(2)*(r-1)+4)/2)*8;  // https://stackoverflow.com/a/14995443/15842840
    // midPointCircleDraw(center_x,center_y,r);

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
            
            double center_dist = (double) sqrt ( pow((i-center_x),2) + pow((j-center_y),2) );
            
            // https://stackoverflow.com/a/839931/15842840
            // Get the max distance for each point to normalize the distance between square and circles perimeter
            // x = cx + r * cos(a)
            // y = cy + r * sin(a)
            double max_dist = (double) sqrt ( pow((i-center_x),2) + pow((j-center_y),2) );
            bool condition = center_dist > (double)r ;
            
            // camBinaryMask.at<double>(j,i) = 0.5;

            //(x - a)**2 + (y - b)**2 == r**2;
            // double term1 = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2));
            // double term2 = pow(r,2);
            // bool condition2 = term1 >= term2;
            // bool condition2 = (pow((i - cameraW/2),2) + pow((j - cameraH/2),2)) >= 

            if (condition){

                // double transparency = (center_dist-(double)r)/ (double)abs(cameraW-cameraH);
                double transparency = (center_dist-(double)r)/ (double)sqrt(pow(cameraW-cameraH,2));
                // std::cout<<transparency <<" = "<<center_dist<<"-"<<r<<"("<<center_dist-r<<") / "<<abs(cameraW-cameraH)<<std::endl;
                camBinaryMask.at<double>(j,i) = transparency;

                if (max_dist == r+4 || center_dist == r+3 || center_dist == r+1 || center_dist == r+2 || center_dist == r){
                    int T = (H - cameraH)/2;
                    int L = (W - cameraW)/2;
                    int x = L + i;
                    int y = T + j;
                    // x = i;
                    // y=j;
                    visualFrame.at<cv::Vec3b>(y,x)[0] = 137;
                    visualFrame.at<cv::Vec3b>(y,x)[1] = 137;
                    visualFrame.at<cv::Vec3b>(y,x)[2] = 137;
                }
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

bool Visualizer::_showFrame(){ //bool nativeWindow

    // if (nativeWindow){
    //     std::cout<<"Visualizer : is VISUAL Frame"<<std::endl;
    // }
    // else {
    //     std::cout<<"Visualizer : is CAMERA Frame"<<std::endl;
    // }
    // cv::imshow("Mask", camBinaryMask);                       //Showing the video//
    cv::imshow("Interactive Audio Visualizer", visualFrame);    //Showing the video//
    // cv::waitKey(1);                                          //Allowing 1 milliseconds frame processing time
    if (cv::waitKey(1) == 113) return true;
    return false;

}

void Visualizer::_set_BG_manually(int tone, bool trackEnabled){    // naive

    // #include wavelengths.h
    double percent;

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

        // int B = visualFrame.at<cv::Vec3b>(0,0)[0];
        // int G = visualFrame.at<cv::Vec3b>(0,0)[1];
        // int R = visualFrame.at<cv::Vec3b>(0,0)[2];
        
        int B,G,R;
        if (tone>fmin && tone<=300){                // keep blue             
            percent = (double)tone/300.;  // high trans
            B = 255;
            G = (255.*percent);
            R = (255.*(1.-percent));
        }
        else if (tone >300 && tone <=700){       // keep green
            percent = (double)tone/700.; // low trans
            B = (255.*percent);
            G = 255;
            R = (255.*(1.-percent));
        }
        else if (tone >700 && tone <=fmax) {
            percent = (double)tone/(double)fmax;            
            B = (255.*percent);
            G = (255.*(1.-percent));
            R = 255;
        }
        std::cout<<"tone freq "<<tone<< " percent "<<percent<<std::endl;
        visualFrame.setTo( cv::Scalar( B, G, R ) );
}

void Visualizer::drawSmallcircle(int cameraW, int cameraH, int roicenterX, int roicenterY, int roiVolumeW, int roiVolumeH){

    LR = W - cameraW;
    TB = H - cameraH;

    int center_x = LR/2 + roicenterX; //  
    int center_y = TB/2 + roicenterY; // H/2;
    float transpose_ratio_x = (float)W / (float)cameraW / 2.;
    float transpose_ratio_y = (float)H / (float)cameraH / 2.;

    if ( center_x < W/2 ) //-W*2/3)
        center_x -= transpose_ratio_x*(W/2 - center_x);
    else if ( center_x > W/2) //*2/3)
        center_x += transpose_ratio_x*(center_x - W/2);
    if ( center_y < H/2 )
        center_y -= transpose_ratio_y*(H/2 - center_y);
    else if ( center_y > H/2 )
        center_y += transpose_ratio_y*(center_y - H/2);

    cv::Point center(center_x, center_y);//Declaring the center point
    int radius = roiVolumeW > roiVolumeH ? roiVolumeW/2 : roiVolumeH/2; //Declaring the radius
    cv::Scalar line_Color(0, 0, 0);//Color of the circle
    int thickness = 2;//thickens of the line
    circle(visualFrame, center,radius, line_Color, thickness);//Using circle()functi

}

void Visualizer::draWaveform(int cameraW,int cameraH){

    int x_centre = W/2; 
    int y_centre = H/2; // H/2;
    int r = (cameraW>cameraH) ? cameraH/2 : cameraW/2;

    float min,max;
    int numSamples;
    float *wave = wf.getWaveform(min,max,numSamples);
    std::cout<<"Visualizer::_set_FG_manually --> min:"<<min<<" max:"<<max<<" numSamples:"<<numSamples<<" wave[0]:"<<wave[0]<<std::endl;
// depict waveform
    // double min,max;
    // int numSamples;
    // // update_wave_frame(start,end);
    // double *wave=wf.getWaveform(min,max,numSamples);

// works but no circular waveform - keep for debugging
    // int start=0;
    // int end=W;
    // std::cout<<"numSamples "<<numSamples<<" circleMask.size() "<<circleMask.size()<<std::endl;
    // if (numSamples<W){
    //     end=numSamples;
    // }else end = W;
    // int counter=0;
    // for (int x_trans = start; x_trans < end ; x_trans++ ){
    //     // double percent = (wave[counter] - min / max - min);
    //     // int ytr = H/2 + (double)H/3*percent;
    //     // normalize in -1 1
    //     float percent = 2* ( (wave[counter]-min) / (max-min) ) -1;
    //     int ytr = H/2 + 3.0*percent;
    //     // int ytr = (double)H/2. + (double)H*100.*wave[counter];
    //     // std::cout<<" > x,y "<<x_trans<<","<<wave[counter]<<"--> normalized value = "<<ytr<< "with percent = "<<percent<<" , minmax=("<<min<<","<<max<<")"<<std::endl;
    //     // std::cout<<"filling foreground in ("<<x_trans<<","<<ytr<<")"<<std::endl;
    //     visualFrame.at<cv::Vec3b>(ytr,x_trans)[0] = 255;//newval[0];
    //     visualFrame.at<cv::Vec3b>(ytr,x_trans)[1] = 255;//newval[1];
    //     visualFrame.at<cv::Vec3b>(ytr,x_trans)[2] = 255;//newval[2];
    //     counter++;
    // }

// start working here ........................................................................................................................
//     int start=0;
//     int end;
//     if (numSamples<circleMask.size()){
//         end=numSamples;
//     }else end = circleMask.size();
//     int x = r, y = 0;
//     // Printing the initial point on the axes 
//     // after translation
//     // cout << "(" << x + x_centre << ", " << y + y_centre << ") ";
//     // When radius is zero only a single
//     // point will be printed
//     if (r > 0)
//     {
// // -y + y_centre , x + x_centre 
// //  x + y_centre , y + x_centre 
// //  x + y_centre ,-y + x_centre 
//         visualFrame.at<cv::Vec3b>(-y + y_centre , x + x_centre )[0] = 0;
//         visualFrame.at<cv::Vec3b>( x + y_centre , y + x_centre )[1] = 0;
//         visualFrame.at<cv::Vec3b>( x + y_centre ,-y + x_centre )[2] = 0;
//         cv::putText(visualFrame, "1", cv::Point( x + x_centre ,-y + y_centre) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 255), 1, cv::LINE_AA); //  ,font_Color, font_weight);
//         cv::putText(visualFrame, "1", cv::Point( y + x_centre , x + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
//         cv::putText(visualFrame, "1", cv::Point(-y + x_centre , x + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
//     }
//     // Initialising the value of P
//     int P = 1 - r;
//     while (x > y)
//     { 
//         y++; 
//         // Mid-point is inside or on the perimeter
//         if (P <= 0)
//             P = P + 2*y + 1;
//         // Mid-point is outside the perimeter
//         else
//         {
//             x--;
//             P = P + 2*y - 2*x + 1;
//         }
//         // All the perimeter points have already been printed
//         if (x < y)
//             break;
//         // Printing the generated point and its reflection
//         // in the other octants after translation
//         visualFrame.at<cv::Vec3b>( y + y_centre , x + x_centre )[0] = 137;
//         visualFrame.at<cv::Vec3b>( y + y_centre ,-x + x_centre )[1] = 137;
//         visualFrame.at<cv::Vec3b>(-y + y_centre , x + x_centre )[2] = 137;
//         visualFrame.at<cv::Vec3b>(-y + y_centre ,-x + x_centre )[2] = 137;
//         // cv::putText(visualFrame, "2", cv::Point( x + x_centre , y + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
//         // cv::putText(visualFrame, "2", cv::Point(-x + x_centre , y + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
//         // cv::putText(visualFrame, "2", cv::Point( x + x_centre ,-y + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
//         // cv::putText(visualFrame, "2", cv::Point(-x + x_centre ,-y + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
//         // If the generated point is on the line x = y then 
//         // the perimeter points have already been printed
//         if (x != y)
//         {
//             visualFrame.at<cv::Vec3b>( x + y_centre , y + x_centre )[0] = 137;
//             visualFrame.at<cv::Vec3b>( x + y_centre ,-y + x_centre )[1] = 137;
//             visualFrame.at<cv::Vec3b>(-x + y_centre , y + x_centre )[2] = 137;
//             visualFrame.at<cv::Vec3b>(-x + y_centre ,-y + x_centre )[2] = 137;
//             // cv::putText(visualFrame, "3", cv::Point( y + x_centre , x + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
//             // cv::putText(visualFrame, "3", cv::Point(-y + x_centre , x + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
//             // cv::putText(visualFrame, "3", cv::Point( y + x_centre ,-x + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
//             // cv::putText(visualFrame, "3", cv::Point(-y + x_centre ,-x + y_centre ) ,cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
//         }
//         if (static_counter<=10){
//             std::string test =std::to_string(static_counter);
//             std::string impath = "/home/blackhawk/interactive-audio-visualizer/src/"+test+".jpg";
//             std::cout<<"impath "<<impath<<std::endl;
//             imwrite(impath, visualFrame); // A JPG FILE IS BEING SAVED
//             static_counter++;
//         }
//     }
//     std::cout<<"Vector length "<< circleMask.size()<<std::endl;

// directly to solution 2
    int start=0;
    int end=numberofpixels;
    double waveSamplingRatio = (double)numSamples / (double)numberofpixels;
    double curRadians=0.0;
    double radianStep=2*M_PI / (double)numberofpixels;

    // if (numSamples<numberofpixels){
    //     end=numSamples;
    // }else end = numberofpixels;

    // std::cout<<"numberofpixels "<<numberofpixels<<" numSamplesWaveform "<<numSamples<<std::endl;
    int counter = 0;
    int x_trans,ytr;
    int thickness=1;
    for (int i=start; i<end ; i++){
        // pixels are calculated given the following equations
        // x = cx + r * cos(a)
        // y = cy + r * sin(a)
        int x = (float)r * std::cos(curRadians) + (float)x_centre;
        int y = (float)r * std::sin(curRadians) + (float)y_centre;
        // std::cout<<"x,y------------------------------------------->"<<x<<","<<y<<std::endl;

    // for (int x_trans = start; x_trans < end ; x_trans++ ){
        // normalize in -1 1
        float percent = 2* ( (wave[counter]-min) / (max-min) ) -1;
        ytr = H/2 + 3.0*percent;

        // trasport x and y
        float new_radius = r+((float)H/40.*percent);
        int x2 = new_radius * std::cos(curRadians) + (float)x_centre;
        int y2 = new_radius * std::sin(curRadians) + (float)y_centre;
        cv::Point p1(x, y), p2(x2, y2); 
        cv::line(visualFrame, p1, p2, cv::Scalar(255, 0, 0), thickness, cv::LINE_8); 

        counter++;
        if (counter>numSamples) counter=0;

        // visualFrame.at<cv::Vec3b>( y , x )[0] = 137;
        // visualFrame.at<cv::Vec3b>( y , x )[1] = 137;
        // visualFrame.at<cv::Vec3b>( y , x )[2] = 137;
        curRadians+=radianStep;
    }
    
}

// works but no circular waveform - keep for backup
// void Visualizer::__draWaveform(){
//     int center_x = W/2; 
//     int center_y = H/2; // H/2;
//     float min,max;
//     int numSamples;
//     float *wave = wf.getWaveform(min,max,numSamples);
//     std::cout<<"Visualizer::_set_FG_manually --> min:"<<min<<" max:"<<max<<" numSamples:"<<numSamples<<" wave[0]:"<<wave[0]<<std::endl;
// // depict waveform
//     // double min,max;
//     // int numSamples;
//     // // update_wave_frame(start,end);
//     // double *wave=wf.getWaveform(min,max,numSamples);
//     int start=0;
//     int end=W;
//     if (numSamples<W){
//         end=numSamples;
//     }else end = W;
//     // if (min!=0 && max!=0){
//         int counter=0;
//         for (int x_trans = start; x_trans < end ; x_trans++ ){
//             // double percent = (wave[counter] - min / max - min);
//             // int ytr = H/2 + (double)H/3*percent;
//             // normalize in -1 1
//             float percent = 2* ( (wave[counter]-min) / (max-min) ) -1;
//             int ytr = H/2 + 3.0*percent;
//             // int ytr = (double)H/2. + (double)H*100.*wave[counter];
//             // std::cout<<" > x,y "<<x_trans<<","<<wave[counter]<<"--> normalized value = "<<ytr<< "with percent = "<<percent<<" , minmax=("<<min<<","<<max<<")"<<std::endl;
//             // std::cout<<"filling foreground in ("<<x_trans<<","<<ytr<<")"<<std::endl;
//             visualFrame.at<cv::Vec3b>(ytr,x_trans)[0] = 255;//newval[0];
//             visualFrame.at<cv::Vec3b>(ytr,x_trans)[1] = 255;//newval[1];
//             visualFrame.at<cv::Vec3b>(ytr,x_trans)[2] = 255;//newval[2];
//             counter++;
//         }
//     // }
// }

void Visualizer::_set_FG_manually(cv::Mat cameraFrame , RegionOfInterest roi){
// SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT SKILLPOINT 
    // int L = roi.centerX - roi.volumeW/2;
    // int T = roi.centerY - roi.volumeH/2;
    // cameraFrame.copyTo(visualFrame(cv::Rect(L,T,roi.volumeW, roi.volumeH))); // cols, rows
// cameraFrame.cols
// cameraFrame.rows
    
// wrong center
    // int L = (W - cameraFrame.cols)/2;
    // int T = (H - cameraFrame.rows)/2;
    // int centerX = L + roi.centerX;
    // int centerY = T + roi.centerY;
    // cv::Point center(centerX, centerY);//Declaring the center point
    // int radius = roi.volumeW > roi.volumeH ? roi.volumeW/2 : roi.volumeH/2; //Declaring the radius
    // cv::Scalar line_Color(0, 0, 0);//Color of the circle
    // int thickness = 2;//thickens of the line
    // circle(visualFrame, center,radius, line_Color, thickness);//Using circle()functi

// Simple Debugged
    // int LR = W - cameraFrame.cols;
    // int TB = H - cameraFrame.rows;
    // int center_x = LR/2 + roi.centerX; //  
    // int center_y = TB/2 + roi.centerY; // H/2;
    // cv::Point center(center_x, center_y);//Declaring the center point
    // int radius = roi.volumeW > roi.volumeH ? roi.volumeW/2 : roi.volumeH/2; //Declaring the radius
    // cv::Scalar line_Color(0, 0, 0);//Color of the circle
    // int thickness = 2;//thickens of the line
    // circle(visualFrame, center,radius, line_Color, thickness);//Using circle()functi


// 

    drawSmallcircle(cameraFrame.cols,cameraFrame.rows,roi.centerX,roi.centerY,roi.volumeW,roi.volumeH);





    // int L = (W - cameraFrame.cols)/2;
    // int T = (H - cameraFrame.rows)/2;
    // int centerX = L + roi.centerX;
    // int centerY = T + roi.centerY;
    // int centerX = roi.centerX;
    // int centerY = roi.centerY;
    // float transpose_ratio_x = (float)W / (float)cameraFrame.cols / 2.;
    // float transpose_ratio_y = (float)H / (float)cameraFrame.rows / 2.;
    // if (framecounter < )
    // if (centerX < W/2 - 4*roi.volumeW ) //-W*2/3)
    //     centerX -= transpose_ratio_x*(W/2 - centerX);
    // else if (centerX < W/2 + 4*roi.volumeW ) //*2/3)
    //     centerX += transpose_ratio_x*(centerX - W/2);
    // if (centerY < H/2 - 4 * roi.volumeH )
    //     centerY -= transpose_ratio_y*(H/2 - centerY);
    // else if (centerY > H/2 + 4 * roi.volumeH )
    //     centerY += transpose_ratio_y*(centerY - H/2);
    // cv::Point center(centerX, centerY);//Declaring the center point
    // int radius = roi.volumeW > roi.volumeH ? roi.volumeW/2 : roi.volumeH/2; //Declaring the radius
    // cv::Scalar line_Color(0, 0, 0);//Color of the circle
    // int thickness = 2;//thickens of the line
    // circle(visualFrame, center,radius, line_Color, thickness);//Using circle()functi


    // cv::Point center(centerX, centerY);//Declaring the center point
    // int radius = roi.volumeW > roi.volumeH ? roi.volumeW/2 : roi.volumeH/2; //Declaring the radius
    // cv::Scalar line_Color(0, 0, 0);//Color of the circle
    // int thickness = 2;//thickens of the line
    // circle(visualFrame, center,radius, line_Color, thickness);//Using circle()functi

    draWaveform(cameraFrame.cols,cameraFrame.rows);
    
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

    // std::cout<<"circleMask "<<circleMask.size()<<std::endl;

    // for (auto m : circleMask){
    //     // cameraFrame.at<cv::Vec3b>(m.second,m.first)[0] = 70;
    //     // cameraFrame.at<cv::Vec3b>(m.second,m.first)[1] = 70;
    //     // cameraFrame.at<cv::Vec3b>(m.second,m.first)[1] = 70;
    //     std::cout<<"circleMask "<<m.first<<","<<m.second<<std::endl;
    // }
    // cv::Rect rect(0,0,cameraFrame.cols, cameraFrame.rows);
    // cv::Mat extractedImage2 = videoframe(rect);
    // cameraFrame.copyTo(videoframe(rect1));

    double vB = (double)visualFrame.at<cv::Vec3b>(0,0)[0];
    double vG = (double)visualFrame.at<cv::Vec3b>(0,0)[1];
    double vR = (double)visualFrame.at<cv::Vec3b>(0,0)[2];

    for (int i=0;i<cameraW;i++){
        for (int j=0;j<cameraH;j++){
            
            
            // std::cout<<"cameraFrame.at<cv::Vec3b>(j,i)[0] == "<<cameraFrame.at<cv::Vec3b>(j,i)[0]<<std::endl;
            // cameraFrame.at<cv::Vec3b>(j,i)[0] = cameraFrame.at<cv::Vec3b>(j,i)[0]*(1.-ratio) + (ratio*visualFrame.at<cv::Vec3b>(T+j,L+i)[0]);
            // cameraFrame.at<cv::Vec3b>(j,i)[1] = cameraFrame.at<cv::Vec3b>(j,i)[1]*(1.-ratio) + (ratio*visualFrame.at<cv::Vec3b>(T+j,L+i)[1]);
            // cameraFrame.at<cv::Vec3b>(j,i)[2] = cameraFrame.at<cv::Vec3b>(j,i)[2]*(1.-ratio) + (ratio*visualFrame.at<cv::Vec3b>(T+j,L+i)[2]);
            if (camBinaryMask.at<double>(j,i)>0.){
                double ratio = camBinaryMask.at<double>(j,i);
                // ratio = 1.0-ratio;
                // works
                // cameraFrame.at<cv::Vec3b>(j,i)[0] = ((double)cameraFrame.at<cv::Vec3b>(j,i)[0]*(1.-ratio)) + (ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[0]);
                // cameraFrame.at<cv::Vec3b>(j,i)[1] = ((double)cameraFrame.at<cv::Vec3b>(j,i)[1]*(1.-ratio)) +(ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[1]);
                // cameraFrame.at<cv::Vec3b>(j,i)[2] = ((double)cameraFrame.at<cv::Vec3b>(j,i)[2]*(1.-ratio)) + (ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[2]);
                

                // cameraFrame.at<cv::Vec3b>(j,i)[0] = (ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[0]);
                // cameraFrame.at<cv::Vec3b>(j,i)[1] = (ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[1]);
                // cameraFrame.at<cv::Vec3b>(j,i)[2] = (ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[2]);
                
                // double ratio2 = 1.0-ratio;
                // std::cout<<(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[0]<<"*"<<ratio2<<"=="<<(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[0]*ratio2<<std::endl;
                // std::cout<<(double)cameraFrame.at<cv::Vec3b>(j,i)[0]<<"*"<<ratio<<"=="<<(double)cameraFrame.at<cv::Vec3b>(j,i)[0]*ratio<<std::endl;
                
                // cameraFrame.at<cv::Vec3b>(j,i)[0] = ratio*255.;
                // cameraFrame.at<cv::Vec3b>(j,i)[1] = ratio*255.;
                // cameraFrame.at<cv::Vec3b>(j,i)[2] = ratio*255.;
                
                //works -> gives grey scale result
                // cameraFrame.at<cv::Vec3b>(j,i)[0] = ((ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[0]) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[0])/2.;
                // cameraFrame.at<cv::Vec3b>(j,i)[1] = ((ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[1]) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[1])/2.;
                // cameraFrame.at<cv::Vec3b>(j,i)[2] = ((ratio*(double)visualFrame.at<cv::Vec3b>(T+j,L+i)[2]) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[2])/2.;
                
                cameraFrame.at<cv::Vec3b>(j,i)[0] = ((ratio*vB) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[0])/2.;
                cameraFrame.at<cv::Vec3b>(j,i)[1] = ((ratio*vG) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[1])/2.;
                cameraFrame.at<cv::Vec3b>(j,i)[2] = ((ratio*vR) + (1.-ratio)*(double)cameraFrame.at<cv::Vec3b>(j,i)[2])/2.;
                
            }
            
        }
    }

    // std::cout<<"hello there"<<std::endl;
    cameraFrame.copyTo(visualFrame(cv::Rect(L,T,cameraFrame.cols, cameraFrame.rows)));

}

int Visualizer::and_Sound_into_Image(float* left, float* right,cv::Mat videoframe, bool frameElapsed, bool trackEnabled, int tone,RegionOfInterest roi){
    
    bool exit_msg=false;

    if (trackEnabled) {  //                                    <-- only for DEBUG, REMOVE IF YOU WAN T TO PROCESS BG MUSIC - apart from sine waves
        // std::cout<<"trackEnabled "<<trackEnabled<< " left 0,511:"<<left[0]<<","<<left[511]<<" right 0,511:"<<right[0]<<","<<right[511]<<std::endl;
        wf.prepare_waveform(left,right); //--> addressed to the next update
        sp.prepare_spectrogram((double*)left,(double*)right);
    }

    if (frameElapsed){                                    // is this a legitimate solution? otherwise try threads
    
        if (trackEnabled){ // preprocess visual_frame -->   doesn't depict the frame, it just edits it so it does not require a new frame to be captured by the camera.
            // update the current visualframe according to the changing of the tracking stimulus
            _set_BG_manually(tone, trackEnabled);
            // update_spectrogram();
            _set_FG_manually(videoframe,roi);
        }else{
            _setToCamera(videoframe);
        }

        exit_msg = _showFrame();   // shows the video captured from camera --> where should it depict the visualFrame?

    }

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

int Visualizer::update_spectrogram(){
    // dft=
    double minf,maxf;
    sp.computeFFT(dft,minf,maxf);
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