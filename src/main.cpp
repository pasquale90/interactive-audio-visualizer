#include "audio.h"
#include "fft.h" // MAKE IT A CLASS?
#include "visualizer.h"
#include <iostream>
#include <cstdlib>
#include <string>


int WIDTH=1000;
int HEIGHT=700;
float audioBuffer{NULL};
int SAMPLE_RATE;
int BUFFER_SIZE;
Visualizer vs;

void audioBufferCallback(float* in){
    // audioBuffer=*in;
    std::cout<<"main in "<<*in<<std::endl;
    // computeFFT(in,BUFFER_SIZE);
    
    // updateFrame(in,NULL);
    vs.stream_frames(in);
    // Visualizer2(WIDTH,HEIGHT);
}

int main(int argc,char **argv){

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Visualizer"<<std::endl;
    std::cout<<"\n\n";

    SAMPLE_RATE=std::stoi(argv[1]);
    BUFFER_SIZE=std::stoi(argv[2]);
    std::cout<<"SAMPLE RATE = "<<SAMPLE_RATE<<std::endl;
    std::cout<<"BUFFER_SIZE = "<<BUFFER_SIZE<<std::endl;

    vs=Visualizer(WIDTH,HEIGHT,SAMPLE_RATE,BUFFER_SIZE);

    const char* serverName=NULL;
    const char* clientName="myAudioStream"; 
    AudioStream *myAudioStrem = new AudioStream(serverName,clientName); //NULL,"myAudioStream"

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Stream"<<std::endl;
    std::cout<<"\n\n";
    myAudioStrem->AudioRouting();
    // myAudioStrem.streamAudio(); //bufferLeft,bufferRight
    myAudioStrem->closeStream();

    std::cout<<"\n\n";
    std::cout<<"Reached the end of main"<<std::endl;
    std::cout<<"\n\n";

    return 0;
}