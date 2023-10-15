#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

/*
* here translation of the tracked visual signal into audio sine waves takes place
* recieve data from the tracking.get_data() method and return int frequency
*/

#include "signal.h"

class Audiolizer{

public:

    Audiolizer();
    ~Audiolizer();


    void setConfig(int);
    bool get_signal(int&);

    void _simple_definition(int& freq);
    void _simple_freqRange_palindrome(int& freq);
    void turn_Image_into_Sound(int& freq);

private:

    int bufferSize;
    bool tempAscenting;
    int tempFreqcounter;
};


#endif