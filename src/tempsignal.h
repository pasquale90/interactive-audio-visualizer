#ifndef TEMPSIGNAL_INCLUDED
#define TEMPSIGNAL_INCLUDED

#include <iostream>
#include <vector>
//class AudioFile; //Forward declaration
#include "AudioFile.h"

class TempSignal{
  public:
    TempSignal();
    //TempSignal(const char* filepath);
    TempSignal(std::string filepath);
    ~TempSignal();

    std::vector<double> stream();
    std::vector<double> get_buffer();




    void print_samples();
    void info();

  protected:
    AudioFile<double> audioread();
  private:
    AudioFile<double> signal;
    AudioFile<double>::AudioBuffer buffer;
    //std::vector<double> *buffer;
};

#endif
