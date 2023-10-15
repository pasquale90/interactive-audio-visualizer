#include "beatracker.h"
#include <iostream>
#include <typeinfo>

Beatracker::Beatracker(){
    isdownbeat=false;
    std::cout<<"Beatracker awaits for buffer_size initialization"<<buffer_size<<std::endl;
}

Beatracker::Beatracker(int bufferSize) :  buffer_size(bufferSize) { 
    BTrack btracker(bufferSize/2, bufferSize);
    
    // std::queue<double> energyEnvelopeList;
    isdownbeat=false;
    std::cout<<"Beatracker initialized with buffer size "<<buffer_size<<std::endl;
}

Beatracker::~Beatracker(){
    btracker.~BTrack();
    std::cout<<"Beatracker object destructed"<<std::endl;
}

void Beatracker::setConfig(const Config& cfg){
    BTrack btracker(cfg.bufferSize/2, cfg.bufferSize);
    std::cout<<"Beatracker initialized with buffer size "<<buffer_size<<std::endl;
}

void Beatracker::check_downbeat(double *buffer){ // assume 4 beats per bar --> actually returns true if it is the maximum energy envelope of the last 4 beats
    
    //calculate the energy envelope of the signal
    double sum = 0;
    // sum the squares of the samples
    for (int i = 0;i < buffer_size;i++) 
        sum += (buffer[i] * buffer[i]);


    energyEnvelopeList.push(sum);
    if (energyEnvelopeList.size()==9) energyEnvelopeList.pop();
    for (int i=0;i<energyEnvelopeList.size();i++){
        if (sum>energyEnvelopeList.front())
        {
            isdownbeat=true;
            return;
        }
    }
    isdownbeat=false;

    // std::cout<<"sum "<<sum<<std::endl;
    // btracker.processOnsetDetectionFunctionSample(sum/buffer_size);
    // isdownbeat = btracker.beatDueInCurrentFrame();
}

bool Beatracker::isBeat(double *buffer){
    btracker.processAudioFrame(buffer);
    bool isbeat=btracker.beatDueInCurrentFrame();

    if (isbeat) 
        check_downbeat(buffer);

    return isbeat;
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

bool Beatracker::isDownbeat(){
    return isdownbeat;
}

// void Beatracker::getFFT(){
//     doublebuff=btracker.getFFTbuffer();
    
//     // btracker.getFFTbuffer(doublebuff);
//     std::cout<<"doublebuff[0] "<<doublebuff[0]<<std::endl;
//     for (int i = 0; i < btracker.getFFTlen(); i++)
// 		printf("BeatTracker : %3d %+9.5f I \n",i, doublebuff[i]);
// 	printf("\n\n\n\n\n");

// }