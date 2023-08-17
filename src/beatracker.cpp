#include "beatracker.h"
#include <iostream>
#include <typeinfo>

Beatracker::Beatracker(){
}

Beatracker::Beatracker(int bufferSize) :  buffer_size(bufferSize) { 
    BTrack btracker(bufferSize);
    std::cout<<"Beatracker initialized with buffer size "<<buffer_size<<std::endl;
}

Beatracker::~Beatracker(){
    btracker.~BTrack();
}

bool Beatracker::isBeat(double *buffer){
    btracker.processAudioFrame(buffer);
    return btracker.beatDueInCurrentFrame();
}

bool Beatracker::isOnset(double *buffer){
    
    // double tempbuffer[buffer_size];
    // for (int i = 0; i < buffer_size; i++) tempbuffer[i]= (double) buffer[i];
    // btracker.processOnsetDetectionFunctionSample(tempbuffer[0]);
    
    btracker.processOnsetDetectionFunctionSample(buffer[0]);
    return btracker.beatDueInCurrentFrame();
}

float Beatracker::getCurrTempoEstimate(){
    return btracker.getCurrentTempoEstimate();
}