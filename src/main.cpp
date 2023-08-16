#include "audio.h"
#include "fft.h" // MAKE IT A CLASS?
#include "visualizer.h"
#include "beatracker.h"
#include <iostream>
#include <cstdlib>
#include <string>
// #include <pthread.h>

int WIDTH=1000;
int HEIGHT=700;
float audioBuffer{NULL};
int SAMPLE_RATE;
int BUFFER_SIZE;
Beatracker bt;

Visualizer vs;
// pthread_t visThread;

void audioBufferCallback(float* in){
    // audioBuffer=*in;
    // std::cout<<"main in "<<*in<<std::endl;
    // computeFFT(in,BUFFER_SIZE);
    
    // updateFrame(in,NULL);
    bool isBeat=bt.isBeat(in);
    // bool isBeat=bt.isOnset(in);
    if (isBeat)
    {
        // do something on the beat
        std::cout<<"Beat!"<<std::endl;
    }

    vs.stream_frames(in,isBeat);
    // pthread_join(visThread, NULL);
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

    // Beatracker bt(BUFFER_SIZE);
    Beatracker bt(BUFFER_SIZE);

    std::cout<<"exited beatrackerrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\n\n\n\n "<<std::endl;
    vs=Visualizer(WIDTH,HEIGHT,SAMPLE_RATE,BUFFER_SIZE);
    // pthread_create(&visThread,NULL,&Visualizer::visualize,&vs);
    

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