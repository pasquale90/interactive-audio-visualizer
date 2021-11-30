#include <iostream>

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

#include "tempsignal.h"
int main(){
  std::string filepath="media/birds.wav";
  TempSignal signal=TempSignal(filepath);
  signal.info();
  signal.listener();
  signal.print_samples();
}
