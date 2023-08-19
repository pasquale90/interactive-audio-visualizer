//include ifndef
#ifndef BEATRACKER_H
#define BEATRACKER_H

#include "BTrack.h"
#include <cstring>
#include <queue>

class Beatracker
{
private:
    BTrack btracker;
    int buffer_size;
    double *doublebuff;
    
    bool isdownbeat;
    void check_downbeat(double*);
    std::queue<double> energyEnvelopeList;

public:

    Beatracker(); 
    Beatracker(int);
    ~Beatracker();
    
    bool isBeat(double*);
    bool isOnset(double*);
    bool isDownbeat();

    float getCurrTempoEstimate();
    void getFFT();
};

#endif

            