#ifndef TEMPSIGNAL_INCLUDED
#define TEMPSIGNAL_INCLUDED

#include <iostream>
#include <vector>
//class AudioFile; //Forward declaration
#include "AudioFile.h"

class TempSignal{
  public:
    TempSignal();
    TempSignal(std::string filepath);    //TempSignal(const char* filepath);
    ~TempSignal();

    void info();
    void print_samples();
    void listener();

    int get_num_channels(){return num_channels;};
    int get_buffer_size(){return buffer_size;};
    std::vector<double> get_buffer();
    void stream();    //std::vector<double> stream();


    //move them to protected??
    void set_num_channels(bool stereo);
    void set_buffer_size(int buffer_size);
    void set_buffer();

  protected:
    AudioFile<double> audioread();
    void set_sample_rate(int sample_rate);

  private:
    AudioFile<double> signal;
    AudioFile<double>::AudioBuffer buffer;    //std::vector<double> *buffer;
    int sample_rate;
    int bit_depth;
    int num_samples;
    double lengthInSeconds;
    int num_channels;
    bool is_mono;
    bool is_stereo;
    int buffer_size;
    int buffer_count;//static?
};

#endif
