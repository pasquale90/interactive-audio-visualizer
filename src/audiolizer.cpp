#include "audiolizer.h"

Audiolizer::Audiolizer(){
    tempFreqcounter=200;
    tempAscenting=false;
    // toggleFrame=true;
    cv::namedWindow("1");
}
Audiolizer::~Audiolizer(){
     cv::namedWindow("1");
    camera_tracker.~VideoTracker();
}

void Audiolizer::setConfig(const Config& cfg){
    
    // bufferSize=buffer_size;
    camera_tracker.setConfig(cfg);
    camera_tracker.display_config();

    prev_freq=0;
}

void Audiolizer::_capture(){
    while(true){
        camera_tracker._capture();
    }
}

// bool Audiolizer::turn_Image_into_Sound(int& freq,cv::Mat& roi){
bool Audiolizer::turn_Image_into_Sound(int& freq,cv::Mat& roi){

   /***
    * returns boolean if new frame occured
    * returns by value the frequency that will be streamed on the next audio buffer
   */

    // just for testing ... 
    // _simple_definition(freq);
    // _simple_freqRange_palindrome(freq);

    /*
    if (camera_tracker.get_frame_elapsed()){                    // if a new has elapsed
        // call the tracker to update the roi and roicenter
        if(camera_tracker.update(ROIcenter,roi)){               // if ROI is updated
            if(_translate(freq)){       // if previous frequency has the same value as before it returns the previous frequency
                prev_freq = freq;
                return true;
            }
        }else freq=200;

    }// else freq=prev_freq; // everything just remain the same
    */

    
    bool tracking_updated = camera_tracker.update(ROIcenter,roi);

    if (!roi.empty()){
        imshow("1", roi);
        std::cout<<"turn_Image_into_Sound ISSSSSSSSSSSSSSSSSS NOT EMPTY\n\n\n\n\n\n"<<std::endl;
    }
        
    freq=200;
    return false;
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
    
    //apply translation from x,y to Hz
    _simple_freqRange_palindrome(freq); // <---------------------------------------------------------delete later
    return true;

}