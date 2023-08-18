//include ifndef
#ifndef BEATRACKER_H
#define BEATRACKER_H

#include "BTrack.h"
#include <cstring>

class Beatracker
{
private:
    BTrack btracker;
    int buffer_size;
    double *doublebuff;
public:

    Beatracker(); 
    Beatracker(int);
    ~Beatracker();
    
    bool isBeat(double*);
    bool isOnset(double*);

    float getCurrTempoEstimate();
    void getFFT();
};

#endif

            