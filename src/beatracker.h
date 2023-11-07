//include ifndef
#ifndef BEATRACKER_H
#define BEATRACKER_H

#include "config.h"
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
    void setConfig(const Config&);
    
    bool isBeat(double*);
    bool isOnset(double*);
    bool isDownbeat();

    float getCurrTempoEstimate();
    // void getFFT();
};

#endif

            