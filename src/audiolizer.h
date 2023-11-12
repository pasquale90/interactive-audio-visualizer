#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

/*
* here translation of the tracked visual signal into audio sine waves takes place
* recieve data from the tracking.get_data() method and return int frequency
*/

#include "signal.h"
#include "videotracker.h"

class Audiolizer{

public:

    Audiolizer();
    ~Audiolizer();
    
    void setConfig(const Config& cfg);
    bool turn_Image_into_Sound(bool&,int&,cv::Mat&,RegionOfInterest&, float*, float*);
    void capture();

private:
    bool _tickTock();
    bool _translate(RegionOfInterest,int&);
    void _make_sound(float* , float* , int); // fills the audio buffer with a sine wave
    bool _gradualy_fade(int&);

    Signal sig;
    VideoTracker camera_tracker;
    int bufferSize;
    int prev_freq, minFreq, maxFreq;
    int maxW,maxH;

    void _simple_definition(int& freq); // defines a fixed frequency
    void _simple_freqRange_palindrome(int& freq);   // defines a moving frequency over time
    bool tempAscenting; // used only for testing methods such as this->_simple_definition() && this->_simple_freqRange_palindrome() 
    int tempFreqcounter;
    
    void _init_log_freq_scale(int,int); // currently not used
    void _int2log_freq(int&);  // currently not used
    double a,b;
};

#endif