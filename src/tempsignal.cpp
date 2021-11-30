#include<iostream>
//#include <vector>
//#include "AudioFile.h"
#include "tempsignal.h"

#include <math.h> // somewhere earler (for M_PI and sinf()) ERASE

/*
AudioFile<double> gitexample(){
  // 1. Create an AudioBuffer
  // (BTW, AudioBuffer is just a vector of vectors)
  //already declated in .h file//
  // 2. Set to (e.g.) two channels
  buffer.resize (2);
  // 3. Set number of samples per channel
  buffer[0].resize (100000);
  buffer[1].resize (100000);

  // 4. do something here to fill the buffer with samples, e.g.


  // then...

  int numChannels = 2;
  int numSamplesPerChannel = 100000;
  float sampleRate = 44100.f;
  float frequency = 440.f;

  for (int i = 0; i < numSamplesPerChannel; i++)
  {
    float sample = sinf (2. * M_PI * ((float) i / sampleRate) * frequency) ;

    for (int channel = 0; channel < numChannels; channel++)
         buffer[channel][i] = sample * 0.5;
  }

  // 5. Put into the AudioFile object
  //bool ok = audioFile.setAudioBuffer (buffer);
  return buffer;
}



*/



void TempSignal::print_samples(){
  static int i=0;
  int channel = 0;
  int numSamples = signal.getNumSamplesPerChannel();
  std::cout<<"This file has "<<numSamples<<" for each channel"<<std::endl;

  for (int i = 0; i < numSamples;i++){
    double currentSample = signal.samples[channel][i];
    if (i<20){
      std::cout<<currentSample<<std::endl;
      ++i;
    }
  }
}


void TempSignal::listener(){
  char user_input;
  int stop=0;
  std::cout<<"Press any to stop listening"<<std::endl;
  while (!stop){
    std::cin>>user_input;
    if (std::cin) break;
  }
}

void TempSignal::info(){
  signal.printSummary();
}

TempSignal::TempSignal(std::string filepath){
  this->signal.load(filepath);
  this->sampleRate = this->signal.getSampleRate();
  int bitDepth = this->signal.getBitDepth();

  this->numSamples = this->signal.getNumSamplesPerChannel();
  this->lengthInSeconds = this->signal.getLengthInSeconds();
  this->numChannels = this->signal.getNumChannels();
  this->isMono = this->signal.isMono();
  this->isStereo = this->signal.isStereo();
}

TempSignal::~TempSignal(){}





/*
*/





/*

#ifndef TEMPSIGNAL_INCLUDED
#define TEMPSIGNAL_INCLUDED

//Forward declaration
class AudioFile;

class TempSignal{
public:
  std::vector<double> stream();
  std::vector<double> get_buffer();
protected:
  AudioFile<double> audioread();
private:
  std::vector<double> *buffer;
};
*/
