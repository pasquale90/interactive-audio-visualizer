#include "__audiolizer.h"

Audiolizer::Audiolizer(){
    tempFreqcounter=200;
    tempAscenting=false;
}
Audiolizer::~Audiolizer(){

}

void Audiolizer::setConfig(int buffer_size){
    bufferSize=buffer_size;
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

    turn_Image_into_Sound(freq);
}


void Audiolizer::turn_Image_into_Sound(int& freq){
    //
    // conversion code goes here 
}