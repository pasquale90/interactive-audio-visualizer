#include<iostream>
//#include <vector>
#include "AudioFile.h"
#include "tempsignal.h"

TempSignal::TempSignal(std::string filepath){
  this->signal.load(filepath);
}

TempSignal::~TempSignal(){}

void TempSignal::info(){
  signal.printSummary();
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
