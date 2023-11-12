#include "raw.h"

Waveform::Waveform(){
}

Waveform::~Waveform(){
    delete[] wave;
    std::cout<<"Waveform object destroyed"<<std::endl;
}

void Waveform::set_config(const Config &cfg){
    
    buffer_size=cfg.bufferSize;
    int radius = cfg.radius;
    
    // Using the following operations, calculate a maximum value for wave data to be stored to increase fft resolution:
    // eq : sr / buffers = buffers per second
    // eq : buffers per second / fps = buffers per frame
    float bps = float(cfg.sampleRate) / (float)buffer_size;
    int bpf = std::ceil(bps / (float)cfg.fps) + 1; // add an extra buffer to ensure that there will be no extra audio buffers per frame
    waveLen = bpf*buffer_size;
    sample_counter = 0; // sample counter to reach number of samples added in the waveform stack
    wave=new float[waveLen]; // 2 buffers extra to be sure that there will be no extra buffers in between frames
}

void Waveform::prepare_waveform(float *left, float *right){ // exploit right
    float energy=0.;
    for (int i=0; i < buffer_size ; ++i){
        wave[sample_counter++]=left[i];
        energy+=std::pow(left[i],2);
    }
    float mean_energy = energy/(float)buffer_size;
    // std::cout <<" mean_energy "<< mean_energy<<std::endl;
    if (mean_energy<0.0001){
        sample_counter-=buffer_size; // discards values written in wave by overwritting them in the next audio buffer
    }
}

float* Waveform::getWaveform(float &minf, float &maxf, int &num_samples){
    int end;
    minf=wave[0];
    maxf=wave[0];
    for (int i=0;i<sample_counter;i++){
        if (wave[i]>maxf) maxf=wave[i];
        if (wave[i]<minf) minf=wave[i];
    }
    num_samples=sample_counter;
    float result[sample_counter];
    std::memcpy(result, wave, (sample_counter)*sizeof(float));
    sample_counter=0;
    return wave;
}

int Waveform::getWaveLen(){
    return sample_counter;
}