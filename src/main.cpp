#include "audio.h"
#include "visualizer.h"
#include "beatracker.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>

int WIDTH=1000;
int HEIGHT=700;
int FPS=25;
int SAMPLE_RATE;
int BUFFER_SIZE;
Beatracker *bt;
Visualizer *vs;


int temp=0;
void audioBufferCallback(double* in){
    
    
    
    // updateFrame(in,NULL);
    bool isBeat=bt->isBeat(in);
    
    // bt.getFFT();
    // bool isOnset=bt.isOnset(in);
    // std::cout<<"Current tempo estimate "<<bt.getCurrTempoEstimate()<<std::endl;

    // if (isBeat)
    // {
    //     // do something on the beat
    //     std::cout<<temp%4<<" Beat!"<<std::endl;
    //     temp++;
    // }

    vs->stream_frames(in,isBeat); // showFrame,
}

int main(int argc,char **argv){

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Visualizer"<<std::endl;
    std::cout<<"\n\n";

    SAMPLE_RATE=std::stoi(argv[1]);
    BUFFER_SIZE=std::stoi(argv[2]);
    std::cout<<"SAMPLE RATE = "<<SAMPLE_RATE<<std::endl;
    std::cout<<"BUFFER_SIZE = "<<BUFFER_SIZE<<std::endl;

    bt = new Beatracker(BUFFER_SIZE);
    vs=new Visualizer(WIDTH,HEIGHT,SAMPLE_RATE,BUFFER_SIZE);    

    const char* serverName=NULL;
    const char* clientName="myAudioStream"; 
    AudioStream *myAudioStrem = new AudioStream(serverName,clientName); //NULL,"myAudioStream"

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Stream"<<std::endl;
    std::cout<<"\n\n";
    myAudioStrem->AudioRouting();
    myAudioStrem->closeStream();

    std::cout<<"\n\n";
    std::cout<<"Reached the end of main"<<std::endl;
    std::cout<<"\n\n";

    
   
    bt->~Beatracker();
    vs->~Visualizer();
    return 0;
}
