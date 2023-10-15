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

bool Audiolizer::get_signal(int& freq){

    // conversion code goes here 
    // int FREQUENCY=200;
    // freq=FREQUENCY;

    freq=tempFreqcounter;
    
    
    if (tempAscenting) tempFreqcounter++;
    else {tempFreqcounter--;}
    
    if (tempFreqcounter<=400) tempAscenting=true;
    if (tempFreqcounter>=600) tempAscenting=false;
}
