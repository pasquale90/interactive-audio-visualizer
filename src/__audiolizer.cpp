#include "__audiolizer.h"

Audiolizer::Audiolizer(){
    tempFreqcounter=200;
    tempAscenting=false;
    // toggleFrame=true;
}
Audiolizer::~Audiolizer(){

    camera_tracker.~Tracking();
}

void Audiolizer::setConfig(const Config& cfg){
    
    // bufferSize=buffer_size;
    camera_tracker.setConfig(cfg);
    camera_tracker.display_config();

    prev_freq=0;
}

void Audiolizer::_capture(){
    camera_tracker._capture();
}

bool Audiolizer::turn_Image_into_Sound(int& freq){
   /***
    * returns boolean if new frame occured
    * returns by value the calculated frequency by the turn_Image_into_Sound method
   */

    // just for testing ... 
    // _simple_definition(freq);
    // _simple_freqRange_palindrome(freq);

    // get the frame
    newframeElapsed = _get_frame_elapsed();
    if (newframeElapsed){

        _simple_freqRange_palindrome(freq);
        // prev_freq=freq;                          // if previous visual position same as before return previous frequency
    }else return prev_freq;
    

    return newframeElapsed;
}

bool Audiolizer::_get_frame_elapsed(){
    return camera_tracker.get_frame_elapsed();                                                                            // get_frame --> get_trackingResults()
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