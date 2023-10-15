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


private:

    int bufferSize;
    bool tempAscenting;
    int tempFreqcounter;
};


#endif