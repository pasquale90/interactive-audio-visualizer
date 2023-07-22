//#define filepath "media/audiosample.wav"

/*
void info(Audiofile<double> *audioFile){
  int sr = audioFile->getSampleRate();
  int bd = audioFile->getBitDepth();

  int nums= audioFile->getNumSamplesPerChannel();
  double secs = audioFile->getLengthInSeconds();

  int channels = audioFile->getNumChannels();
  bool isMono = audioFile->isMono();
  bool isStereo = audioFile->isStereo();
  std::cout<<"SampleRate::"<<sr<<"\n"<<"Bitdepth:"<<bd<<std::endl;
  audioFile->printSummary();

}

int main(){
  AudioFile<double> audioFile;
  audioFile.load (audiopath);

  std::cout<<"Hello sketo!"<<std::endl;
  return 0;
}
*/
#include <iostream>
#include "tempsignal.h"
int main(){
  std::string filepath="media/birds.wav";
  TempSignal signal=TempSignal(filepath);
  signal.info();
  signal.listener();
  signal.print_samples();
  signal.set_num_channels(2);
  signal.set_buffer_size(256);
  std::cout<<"Num channels are set to:"<<signal.get_num_channels()<<std::endl;
  std::cout<<"Buffer size is set to:"<<signal.get_buffer_size()<<std::endl;
  std::cout<<"Prepare buffer filling"<<std::endl;

  std::cout<<"NEW CODE SUBMISSION"<<std::endl;
  //signal.set_buffer();
  signal.stream();
  std::cout<<"next step to embed github.com/adamstark/BTrack"<<std::endl;
  return 0;
}
