#include "audio.h"
#include <iostream>

// AudioStream myAudioStream;
// float* bufferLeft;
// float* bufferRight;
// int audioBuffer;

int threading(){
    
}

int main(int argc,void* args){

    AudioStream myAudioStrem = AudioStream(); //NULL,"myAudioStream"
    myAudioStrem.AudioRouting();
    // myAudioStrem.streamAudio(); //bufferLeft,bufferRight
    myAudioStrem.closeStream();
}