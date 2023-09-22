#include "raw.h"



// #include <typeinfo>

Waveform::Waveform(int bufferSize, int buffersPerFrame, int wwidth) : buffer_size(bufferSize), buffers_per_frame(buffersPerFrame), width(wwidth){
    wave=new double[width];
    std::cout<<"Waveform initialized with width "<<width<<std::endl;
}

Waveform::~Waveform(){
    delete[] wave;
    std::cout<<"Waveform object destroyed"<<std::endl;
}

void Waveform::prepare_waveform(int buffCount,double *signal){
    
    double sumBuff=0.000000000;
    
    // prepare signal
    int i;
    for (i=0; i < buffer_size ; ++i){
        sumBuff+=std::pow(signal[i],2)/(double)buffer_size;
    }
    std::cout<<"avgBuff/buffer_size "<<sumBuff<<"/"<<buffer_size<<std::endl;
    wave[buffCount]=std::sqrt(sumBuff); //    /(double)buffer_size;
    // std::cout<<"wave["<<buffCount<<"] "<<wave[buffCount]<<std::endl;
}

double* Waveform::getWaveform(double &minf, double &maxf){
    //simple downsampling;
    minf=wave[0];
    maxf=wave[0];

    for (int i=0;i<buffers_per_frame;i++){
        if (wave[i]>maxf) maxf=wave[i];
        if (wave[i]<minf) minf=wave[i];
        // std::cout<<"new_i "<<new_i<<" i "<<i<<std::endl;
    }

    return wave;
}

// void Waveform::get_ring_xy(int &x, int &y){

// }

int Waveform::getWaveLen(){
    return buffers_per_frame*buffer_size;
}