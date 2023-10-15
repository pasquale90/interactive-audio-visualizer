#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

/*
* here translation of the tracked visual signal into audio sine waves takes place
* recieve data from the tracking.get_data() method and return int frequency
*/

// #include "camera.h"
// #include <thread>
#include "__tracking.h"


class Audiolizer{

public:

    Audiolizer();
    ~Audiolizer();
    void setConfig(const Config& cfg);

    bool turn_Image_into_Sound(int&);

    void _capture();
private:

    bool _get_frame_elapsed();
    
    Tracking camera_tracker;
    int bufferSize;
    bool newframeElapsed;
    int prev_freq;
    
    bool tempAscenting;
    int tempFreqcounter;
    void _simple_definition(int& freq);
    void _simple_freqRange_palindrome(int& freq);    
};


#endif