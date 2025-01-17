#include "audiolizer.h"
#include <math.h>
#include "config.h"
#include "roi.h"

Audiolizer::Audiolizer():cameracfg(Config::getInstance().camconf),iavcfg(Config::getInstance().iavconf){

    // @TEMPORARY DISABLED
    // init_log_freq_scale(); // currently not used. Use _int2log_freq (currently not used either) is affected by this method..

    frequencyRange = iavcfg.maxFrequency - iavcfg.minFrequency;
    prev_freq=0;
    volume = 0.f;
}

void Audiolizer::setAudioUpdater(std::function<void(int, float)> func){
    updateAudio = std::move(func);
}

bool Audiolizer::turn_Image_into_Sound(const bool tracking_updated, const bool pattern_locked , const RegionOfInterest &roi, int& freq){

   /***
    * returns by reference the frequency that will be streamed on the next audio buffer
   */
   
    int prevFreq = prev_freq;

    if (pattern_locked){
        if (tracking_updated)          // if tracking updated --> new x,y --> new freq
            translate(roi, freq);      
        else{                          // else --> previous frequency
            freq=prev_freq;
        } 
    }else{                            // gradualy fade frequency to zero --> if frequency > 0 , slowly decline
        if (freq>1){ 
            gradualy_fade(freq); // gradualy fade frequency to zero --> if frequency > 0 , slowly decline
        }else{
            freq=0;
            volume = 0.f;
        }
    }

    // update audioStream with the newFrequency
    bool frequencyChanged = freq != prevFreq;
    if (frequencyChanged){
        updateAudio(freq , volume);
    }
    
    return frequencyChanged;

}

bool Audiolizer::translate(const RegionOfInterest &roi, int& freq){

    // translate the x coordinate.
    float roiCenterX = static_cast<float>(roi.centerX.load());
    // normalize x, y position 
    float spatial_percent = roiCenterX / static_cast<float>(cameracfg.camResW.load());
    //apply translation from x,y to Hz
    freq = iavcfg.minFrequency + static_cast<int>(spatial_percent* static_cast<float>( frequencyRange ));
    // @TEMPORARY DISABLED
    // int2log_freq(freq); // define here the logarthmic tranformation of the input freq

    // translate the y coordinate 
    float roiCenterY = static_cast<float>(roi.centerY.load());
    // volume ranges from 0.1 up to 0.7 ==> percentage=0.1+(sample×(0.7−0.1)/maxVal)​
    // volume = 0.1f + (( roiCenterY * 0.6f) / static_cast<float>(cameracfg.camResH.load()));
    // volume ranges from 1.0 down to 0.1 ==> percentage=1.0 - (sample×(1.0−0.1)/maxVal)​
    volume= 1.0f - (( roiCenterY * 0.9f) / static_cast<float>(cameracfg.camResH.load()));

    if (freq!=prev_freq){ // if previous frequency has the same value as before it returns the previous frequency
        prev_freq = freq;
        return true;
    }else
        return false;
}

void Audiolizer::gradualy_fade(int& freq){
    if (freq>( frequencyRange )/2) freq -= static_cast<int>(2*log(freq));
    else if (freq<( frequencyRange )/2) freq -= static_cast<int>(log(freq));
    else if (freq<0) freq=0;
}


/* @TEMPORARY DISABLED
void Audiolizer::init_log_freq_scale(){
    
    double minfreq = static_cast<double>(iavcfg.minFrequency);
    double maxfreq = static_cast<double>(iavcfg.maxFrequency);
    double maxW = static_cast<double>(cameracfg.camResW.load());

    // b = log (y2/y1) / (x2-x1) ---> where x1 (minW ==0), x2 (maxW), y1 (minFreq) and y2 (maxFreq)
    b = log (maxfreq / minfreq) / (double)(maxW - 0);
    // a = y2 / exp bx2
    a = maxfreq / (exp(b*(double)maxW));
    
    // given x, find log freq by solving : y = a exp bx
    // ...  (    definition in this->_int2log_freq()     )
}

void Audiolizer::int2log_freq(int &freq){
    // given x, find log freq by solving : y = a exp bx
    freq = a * exp(b * static_cast<double>(freq));
}
*/
