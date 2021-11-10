#include <iostream>
#include <AudioFile.h>

#define audiopath "media/audiosample.wav" 

int main(){
  AudioFile<double> audioFile;
  audioFile.load (audiopath);
  int sr = audioFile.getSampleRate();
  int bd = audioFile.getBitDepth();

  int nums= audioFile.getNumSamplesPerChannel();
  double secs = audioFile.getLengthInSeconds();

  int channels = audioFile.getNumChannels();
  bool isMono = audioFile.isMono();
  bool isStereo = audioFile.isStereo();
  std::cout<<"SampleRate::"<<sr<<"\n"<<"Bitdepth:"<<bd<<std::endl;
  audioFile.printSummary();

  std::cout<<"Hello AV!"<<std::endl;
  return 0;
}
