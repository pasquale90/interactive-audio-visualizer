#include "audiolizer.h"

Audiolizer::Audiolizer(){
    tempFreqcounter=200;
    tempAscenting=false;
    // toggleFrame=true;
    // cv::namedWindow("1");
}
Audiolizer::~Audiolizer(){
    //  cv::destroyWindow("1");
    camera_tracker.~VideoTracker();
}

void Audiolizer::setConfig(const Config& cfg){
    
    // bufferSize=buffer_size;
    camera_tracker.setConfig(cfg);
    camera_tracker.display_config();

    // store 
    maxW = cfg.camResW;
    maxH = cfg.camResH;

    _init_log_freq_scale(cfg.minFrequency,cfg.maxFrequency);
    // minFreq=cfg.minFrequency;
    // maxFreq=cfg.maxFrequency;

    //initialize settings
    prev_freq=0;
    init_frequency=maxFreq-minFreq;
    prev_amp=.0;
    init_amp=0.5;
}

void Audiolizer::_init_log_freq_scale(int minfreq, int maxfreq){
    
    // b = log (y2/y1) / (x2-x1) ---> where x1 (minW ==0), x2 (maxW), y1 (minFreq) and y2 (maxFreq)
    double b = log ((double)maxfreq / (double)minFreq) / (double)(maxW - 0);
    // a = y2 / exp bx2
    double a = (double)maxFreq / (exp(b*(double)maxW));
    
    // given x, find log freq by solving : y = a exp bx
}

void Audiolizer::_capture(){
    while(true){
        camera_tracker._capture();
    }
}

bool Audiolizer::_tickTock(){
    return camera_tracker.tickTock();
}

bool Audiolizer::turn_Image_into_Sound(bool &ispattern, int& freq, cv::Mat& roi){

   /***
    * returns boolean if new frame occured
    * returns by value the frequency that will be streamed on the next audio buffer
   */

    // just for testing ... 
    // _simple_definition(freq);
    // _simple_freqRange_palindrome(freq);

    bool tracking_updated = camera_tracker.update(ROIcenter,roi);
    bool pattern_locked =  camera_tracker._pattern_locked();
    std::cout<<"Audiolizer::turn_Image_into_Sound :: Pattern locked --> "<<camera_tracker._pattern_locked()<<", Tracking updated --> "<<tracking_updated<<std::endl;
    // if (!roi.empty()){
    //     imshow("1", roi);
    //     std::cout<<"turn_Image_into_Sound ISSSSSSSSSSSSSSSSSS NOT EMPTY\n\n\n\n\n\n"<<std::endl;
    // }

    if (pattern_locked){
        if (tracking_updated)          // if tracking updated --> new x,y --> new freq
            // return _translate(freq);        // if previous frequency has the same value as before it returns the previous frequency
            _translate(freq);
        else{                          // else --> previous frequency
            freq=prev_freq;
            // return false;
        } 
        ispattern=true;
    }else{                            // gradualy fade frequency to zero --> if frequency > 0 , slowly decline
        if (freq>0){ 
            // return _gradualy_fade(freq);    // gradualy fade frequency to zero --> if frequency > 0 , slowly decline
            _gradualy_fade(freq);
        }else{
            freq=0;
            // return false;
        }
        ispattern=false;
    }
    return _tickTock();
}

bool Audiolizer::turn_Image_into_Sound_____(int& freq,cv::Mat& roi){

   /***
    * returns boolean if new frame occured
    * returns by value the frequency that will be streamed on the next audio buffer
   */

    // just for testing ... 
    // _simple_definition(freq);
    // _simple_freqRange_palindrome(freq);

    bool tracking_updated = camera_tracker.update(ROIcenter,roi);
    bool pattern_locked =  camera_tracker._pattern_locked();
    std::cout<<"Audiolizer::turn_Image_into_Sound :: Pattern locked --> "<<camera_tracker._pattern_locked()<<", Tracking updated --> "<<tracking_updated<<std::endl;
    if (!roi.empty()){
        imshow("1", roi);
        std::cout<<"turn_Image_into_Sound ISSSSSSSSSSSSSSSSSS NOT EMPTY\n\n\n\n\n\n"<<std::endl;
    }

    if (pattern_locked){
        _translate(freq);
        return true;
        // if (tracking_updated)          // if tracking updated --> new x,y --> new freq
        //     return         // if previous frequency has the same value as before it returns the previous frequency
        // else{                          // else --> previous frequency
        //     freq=prev_freq;
        //     return false;
        // } 
    }else{                            // gradualy fade frequency to zero --> if frequency > 0 , slowly decline
        if (freq>0){ 
            return _gradualy_fade(freq);    
        }else{
            freq=0;
            return false;
        }
    }
}


void Audiolizer::_simple_freqRange_palindrome(int& freq){
    freq=tempFreqcounter;
    if (tempAscenting) tempFreqcounter++;
    else {tempFreqcounter--;}
    
    if (tempFreqcounter<=400) tempAscenting=true;
    if (tempFreqcounter>=600) tempAscenting=false;
}

void Audiolizer::_simple_definition(int& freq){
    int FREQUENCY=200;
    freq=FREQUENCY;
}

bool Audiolizer::_translate(int& freq){
    /***
     * This is where the magic happens. This is where you do the trick
     * This function maps the box potition into a certain frequency. Will be updated using more interaction data (i.e. speed)
    */

    std::cout<<"ROIcenter at ("<<ROIcenter.first<<","<<ROIcenter.second<<")"<<std::endl;
    // normalize x, y position 
    double spatial_percent = (double)ROIcenter.first / (double)maxW;
    //apply translation from x,y to Hz
    // freq = minFreq + (int) (spatial_percent* (double)(maxFreq-minFreq));
    freq = 55 + (int) (spatial_percent* (double)(3520-55));
    std::cout<<"spatial_percent "<<spatial_percent<<" produces frequency "<<freq<<std::endl;

    // given x, find log freq by solving : y = a exp bx
    // freq = (int)(a * exp(b*(double)ROIcenter.first));
    
    if (freq!=prev_freq){
        prev_freq = freq;
        return true;
    }else
        return false;
}

bool Audiolizer::_gradualy_fade(int& freq){
    // gradualy fade frequency to zero --> if frequency > 0 , slowly decline
    std::cout<<"Gradually fade frequency"<<std::endl;

    /*
    if (freq>0){ 
        freq-=3;
        return true;
    }
    // else if (freq==0) 
    else{
        freq=0;
        return false;
    }
    */
    // freq=0; // FOR NOW JUST MAKE IT ZERO -- > FIX LATER        
    freq-=3;
    return true;
}