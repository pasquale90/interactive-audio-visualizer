#include "audio.h"
#include "fft.h" // MAKE IT A CLASS?
#include <iostream>
#include <cstdlib>
#include <string>

// float* bufferLeft;
// float* bufferRight;
float audioBuffer{NULL};
int SAMPLE_RATE;
int BUFFER_SIZE;

void audioBufferCallback(float* in){
    // audioBuffer=*in;
    std::cout<<"main in "<<*in<<std::endl;
    computeFFT(in,BUFFER_SIZE);
}

int main(int argc,char **argv){
    std::cout<<"\n\n";
    std::cout<<"Hello Audio Visualizer"<<std::endl;
    std::cout<<"\n\n";

    SAMPLE_RATE=std::stoi(argv[1]);
    BUFFER_SIZE=std::stoi(argv[2]);
    std::cout<<"SAMPLE RATE = "<<SAMPLE_RATE<<std::endl;
    std::cout<<"BUFFER_SIZE = "<<BUFFER_SIZE<<std::endl;

    const char* serverName=NULL;
    const char* clientName="myAudioStream"; 

    AudioStream *myAudioStrem = new AudioStream(serverName,clientName); //NULL,"myAudioStream"

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Stream"<<std::endl;
    std::cout<<"\n\n";
    myAudioStrem->AudioRouting();
    // myAudioStrem.streamAudio(); //bufferLeft,bufferRight
    myAudioStrem->closeStream();

    return 0;
}