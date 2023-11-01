#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

/*
* here translation of the tracked visual signal into audio sine waves takes place
* recieve data from the tracking.get_data() method and return int frequency
*/

// #include "camera.h"
// #include <thread>
#include "videotracker.h"

struct RegionOfInterest{
    int centerX;
    int centerY;
    int volumeW;
    int volumeH;
};

class Audiolizer{

public:

    Audiolizer();
    ~Audiolizer();
    void setConfig(const Config& cfg);
    bool turn_Image_into_Sound_____(int&,cv::Mat&);
    bool turn_Image_into_Sound(bool&,int&,cv::Mat&,RegionOfInterest&);

    void _capture();

    bool _tickTock();

private:
    
    VideoTracker camera_tracker;
    int prev_freq,init_frequency, minFreq, maxFreq;
    double prev_amp, init_amp;
    int maxW,maxH;
    double a,b;

    std::pair<int,int> ROIcenter;

    // bool _tickTock();

    bool tempAscenting;
    int tempFreqcounter;
    void _simple_definition(int& freq);
    void _simple_freqRange_palindrome(int& freq);   
    bool _translate(int&);
    bool _gradualy_fade(int&);
    void _init_log_freq_scale(int,int);
};


#endif