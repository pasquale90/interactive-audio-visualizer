#include<iostream>
//#include <vector>
//#include "AudioFile.h"
#include "tempsignal.h"

TempSignal::TempSignal(std::string filepath){
  this->signal.load(filepath);
}

TempSignal::~TempSignal(){}

void TempSignal::info(){
  signal.printSummary();
}

void TempSignal::print_samples(){
  static int i=0;
  int channel = 0;
  int numSamples = signal.getNumSamplesPerChannel();
  std::cout<<"This file has "<<numSamples<<" for each channel"<<std::endl;

  for (int i = 0; i < numSamples;i++){
    double currentSample = signal.samples[channel][i];
    if (i<100){
      std::cout<<currentSample<<std::endl;
      ++i;
    }
  }
}
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
