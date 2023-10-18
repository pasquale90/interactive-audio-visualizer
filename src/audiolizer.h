#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

/*
* here translation of the tracked visual signal into audio sine waves takes place
* recieve data from the tracking.get_data() method and return int frequency
*/

// #include "camera.h"
// #include <thread>
#include "videotracker.h"


class Audiolizer{

public:

    Audiolizer();
    ~Audiolizer();
    void setConfig(const Config& cfg);

    bool turn_Image_into_Sound(int&,cv::Mat&);

    void _capture();
private:
    
    VideoTracker camera_tracker;
    int bufferSize;
    bool newframeElapsed;
    int prev_freq;
    
    std::pair<int,int> ROIcenter;

    bool tempAscenting;
    int tempFreqcounter;
    void _simple_definition(int& freq);
    void _simple_freqRange_palindrome(int& freq);   
    bool _translate(int&);
};


#endif