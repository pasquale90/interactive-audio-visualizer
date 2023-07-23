#include "audio.h"
#include <iostream>

// AudioStream myAudioStream;
// float* bufferLeft;
// float* bufferRight;
// int audioBuffer;

// int threading(){
    
// }

int main(int argc,void* args){
    std::cout<<"\n\n";
    std::cout<<"Hello Audio Visualizer"<<std::endl;
    std::cout<<"\n\n";
    const char* serverName=NULL;
    const char* clientName="myAudioStream2"; 

    AudioStream myAudioStrem = AudioStream(serverName,clientName); //NULL,"myAudioStream"
    std::cout<<"\n\n";
    std::cout<<"Hello Audio Stream"<<std::endl;
    std::cout<<"\n\n";
    myAudioStrem.AudioRouting();
    // myAudioStrem.streamAudio(); //bufferLeft,bufferRight
    myAudioStrem.closeStream();

    return 0;
}