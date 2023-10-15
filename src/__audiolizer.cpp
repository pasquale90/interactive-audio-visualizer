#include "__audiolizer.h"

Audiolizer::Audiolizer(){
    tempFreqcounter=200;
    tempAscenting=false;
    toggleFrame=true;
}
Audiolizer::~Audiolizer(){

    camera.~Camera();
}

// void Audiolizer::setConfig(int buffer_size){
//     bufferSize=buffer_size;
// }

void Audiolizer::setConfig(const Config& cfg){
    
    toggleFrame=true;

    // bufferSize=buffer_size;
    camera.setConfig(cfg);
    camera.display_config();

    // mThread = std::thread(&Logger::Run, this);
    // trackingThread = std::thread{&Audiolizer::_capture, this};
    // trackingThread.join();
}

void Audiolizer::_capture(){
    camera.capture();
}

void Audiolizer::_simple_freqRange_palindrome(int& freq){
    freq=tempFreqcounter;
    if (tempAscenting) tempFreqcounter++;
    else {tempFreqcounter--;}
    
    if (tempFreqcounter<=400) tempAscenting=true;
    if (tempFreqcounter>=600) tempAscenting=false;
}

void Audiolizer::_simple_definition(int& freq){
    int FREQUENCY=200;
    freq=FREQUENCY;
}

bool Audiolizer::get_signal(int& freq){
   
    // _simple_definition(freq);
    _simple_freqRange_palindrome(freq);

    // turn_Image_into_Sound(freq);
}

bool Audiolizer::_get_frame(){
    atomicChange=camera.get_frame();
    std::cout<<"atomicChange - toggleFrame "<<atomicChange<<" - "<<toggleFrame<<std::endl;

    if (atomicChange!=toggleFrame){
        // process the current input from camera
        std::cout<<"toggle? "<<"Yes!!"<<std::endl;
        toggleFrame=atomicChange;
        return true;
    }else{

    std::cout<<"toggle? "<<"No :((((((((((((((((((((("<<std::endl;
        return false;
    }

}

void Audiolizer::turn_Image_into_Sound(int& freq){

    // conversion code goes here 


}