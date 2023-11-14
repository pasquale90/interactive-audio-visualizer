#include "audiolizer.h"

Audiolizer::Audiolizer(){
    tempFreqcounter=200;
    tempAscenting=false;
    
    /***
     *  Not defined .... 
    */
}

Audiolizer::~Audiolizer(){
    camera_tracker.~VideoTracker();
}

void Audiolizer::setConfig(const Config& cfg){
    
    //initialize all settings    
    camera_tracker.setConfig(cfg);
    sig.set_config(cfg);
    camera_tracker.display_config();
    maxW = cfg.camResW;
    maxH = cfg.camResH;
    bufferSize = cfg.bufferSize;
    _init_log_freq_scale(cfg.minFrequency,cfg.maxFrequency); // currently not used. Use _int2log_freq (currently not used either) is affected by this method..
    minFreq=cfg.minFrequency;
    maxFreq=cfg.maxFrequency;
    prev_freq=0;
}

void Audiolizer::_init_log_freq_scale(int minfreq, int maxfreq){
    
    // b = log (y2/y1) / (x2-x1) ---> where x1 (minW ==0), x2 (maxW), y1 (minFreq) and y2 (maxFreq)
    b = log ((double)maxfreq / (double)minFreq) / (double)(maxW - 0);
    // a = y2 / exp bx2
    a = (double)maxFreq / (exp(b*(double)maxW));
    
    // given x, find log freq by solving : y = a exp bx
    // ...  (    definition in this->_int2log_freq()     )
}

void Audiolizer::_int2log_freq(int &freq){
    // given x, find log freq by solving : y = a exp bx
    freq = a * exp(b * (double)freq);
}

void Audiolizer::capture(){
    while(true){
        camera_tracker.capture();
    }
}

bool Audiolizer::_tickTock(){
    return camera_tracker.tickTock();
}

bool Audiolizer::turn_Image_into_Sound(bool &ispattern, int& freq, cv::Mat& frame,RegionOfInterest &roi, float *left , float *right){

   /***
    * returns boolean if new frame occured
    * returns by value the frequency that will be streamed on the next audio buffer
   */

    // just for testing ... 
    // _simple_definition(freq);
    // _simple_freqRange_palindrome(freq);

    bool tracking_updated = camera_tracker.update(roi,frame);
    bool pattern_locked =  camera_tracker.pattern_locked();

    if (pattern_locked){
        if (tracking_updated)          // if tracking updated --> new x,y --> new freq
            _translate(roi, freq);      
        else{                          // else --> previous frequency
            freq=prev_freq;
        } 
        ispattern=true;
    }else{                            // gradualy fade frequency to zero --> if frequency > 0 , slowly decline
        if (freq>0){ 
            _gradualy_fade(freq); // gradualy fade frequency to zero --> if frequency > 0 , slowly decline
        }else{
            freq=0;
        }
        ispattern=false;
    }

    _make_sound(freq, left , right);

    return _tickTock();
}

void Audiolizer::_make_sound(int tone, float* left, float* right){

	if (tone>0){
		sig.prepareSine(tone);
		for(int i=0; i<bufferSize; i++ )
		{
			left[i] = sig.getSineL();
            right[i] = sig.getSineR();
		}
    }else {
        for(int i=0; i<bufferSize; i++ )
        {
            left[i] = 0.;
            right[i] = 0.;
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

bool Audiolizer::_translate(RegionOfInterest ROIcenter, int& freq){

    // normalize x, y position 
    double spatial_percent = (double)ROIcenter.centerX / (double)maxW;
    
    //apply translation from x,y to Hz
    freq = minFreq + (int) (spatial_percent* (double)(maxFreq-minFreq));
    
    // _int2log_freq(freq); // define here the logarthmic tranformation of the input freq
    
    if (freq!=prev_freq){ // if previous frequency has the same value as before it returns the previous frequency
        prev_freq = freq;
        return true;
    }else
        return false;
}

bool Audiolizer::_gradualy_fade(int& freq){
    if (freq>(maxFreq-minFreq)/2) freq -= 2*log(freq);
    else if (freq<(maxFreq-minFreq)/2) freq -= log(freq);
    else if (freq<0) freq=0;
    
    return true; // currently not used
}