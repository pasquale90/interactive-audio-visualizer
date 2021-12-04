#include<iostream>
#include "tempsignal.h"
#include <math.h>
#include <iostream>
#include <thread>
#include "timer.h"
//#include <vector>
//#include "AudioFile.h"

void TempSignal::stream(){
  while (num_samples>(buffer_count*buffer_size)){
  //while(buffer_count<(int)num_samples/(buffer_size*buffer_count)
    //reset buffer
    /*
    std::cout<<"buffer_size\t"<<buffer_size
    <<", buffer_count\t"<<buffer_count
    <<", num_samples\t"<<num_samples
    <<", num_samplesx2\t"<<num_samples*2
    <<", multiplication\t"<<buffer_size*buffer_count<<std::endl;
    */
    set_buffer();
    ++buffer_count;
    //if(buffer_count>10)
    break;
  }
}

void TempSignal::set_buffer(){
   //for (int i=0;i<buffer_size;++i){
   for (int i=buffer_count*buffer_size;i<((buffer_count+1)*buffer_size)-1;++i){
    for (int channel=0;channel<num_channels;++channel){
      buffer[channel][i]=signal.samples[channel][i];
      //std::cout<<i<<"\t"<<buffer[channel][i]<<std::endl;
      //std::cout<<i<<"\t"<<signal.samples[channel][i]<<std::endl;
      //std::cout<<(buffer_count*buffer_size)<<", "<<(((buffer_count+1)*buffer_size)-1)<<", "<<i<<std::endl;
    }
  }
}


void TempSignal::set_buffer_size(int buffer_size){
  this->buffer_size=buffer_size;
  buffer.resize (this->num_channels);

  for (int i=0;i<this->num_channels;i++){
    // 3. Set number of samples per channel
    buffer[i].resize (buffer_size);
  }
  //initialize buffer_count
  buffer_count=0;
}

void TempSignal::set_num_channels(bool stereo){
  if(stereo){
    this->num_channels=2;}
  else{
    this->num_channels=1;}
}

void TempSignal::print_samples(){
  //static int i=0;
  int channel = 0;
  std::cout<<"This file has "<<num_samples<<" for each channel"<<std::endl;

  for (int i = 0; i < num_samples;i++){
    double currentSample = signal.samples[channel][i];
    if (i<20){
      std::cout<<currentSample<<std::endl;
      ++i;
    }
  }
}

//RENAME TO stream() INSTEAD?
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
  this->sample_rate = this->signal.getSampleRate();
  this->bit_depth = this->signal.getBitDepth();
  this->num_samples = this->signal.getNumSamplesPerChannel();
  this->lengthInSeconds = this->signal.getLengthInSeconds();
  this->num_channels = this->signal.getNumChannels();
  this->is_mono = this->signal.isMono();
  this->is_stereo = this->signal.isStereo();
}

TempSignal::~TempSignal(){}
