#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

/*
* here translation of the tracked visual signal into audio sine waves takes place
* recieve data from the tracking.get_data() method and return int frequency
*/

#include "camera.h"
// #include <thread>


class Audiolizer{

public:

    Audiolizer();
    ~Audiolizer();


    // void setConfig(int);
    void setConfig(const Config& cfg);
    bool get_signal(int&);

    void _simple_definition(int& freq);
    void _simple_freqRange_palindrome(int& freq);
    void turn_Image_into_Sound(int& freq);

    void _capture();
private:
    bool _get_frame();
    

    Camera camera;
    // std::thread trackingThread;
    
    int bufferSize;
    bool tempAscenting;
    int tempFreqcounter;

    int toggleFrame;
    bool atomicChange;

};


#endif