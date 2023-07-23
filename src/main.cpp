#include "audio.h"
#include <iostream>

// float* bufferLeft;
// float* bufferRight;
float audioBuffer{NULL};
int bufferSize{}; //parameter in args

void audioBufferCallback(float* in){
    // audioBuffer=*in;
    std::cout<<"main in "<<*in<<std::endl;
}

int main(int argc,void* args){
    std::cout<<"\n\n";
    std::cout<<"Hello Audio Visualizer"<<std::endl;
    std::cout<<"\n\n";
    const char* serverName=NULL;
    const char* clientName="myAudioStream23423"; 

    AudioStream *myAudioStrem = new AudioStream(serverName,clientName); //NULL,"myAudioStream"

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Stream"<<std::endl;
    std::cout<<"\n\n";
    myAudioStrem->AudioRouting();
    // myAudioStrem.streamAudio(); //bufferLeft,bufferRight
    myAudioStrem->closeStream();

    return 0;
}