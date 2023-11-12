#include "raw.h"



// #include <typeinfo>

Waveform::Waveform(int bufferSize, int radius, int wwidth) : buffer_size(bufferSize), W(wwidth){
    wave=new float[W];
}

Waveform::Waveform(){
}

Waveform::~Waveform(){
    delete[] wave;
    std::cout<<"Waveform object destroyed"<<std::endl;
}

void Waveform::set_config(const Config &cfg){
    
    buffer_size=cfg.bufferSize;
    int radius = cfg.radius;
    
    W = cfg.displayW;
    H = cfg.displayH;
    
    // sr / buffers = buffers per second
    // buffers per second / fps = buffers per frame
    float bps = float(cfg.sampleRate) / (float)buffer_size;
    int bpf = std::ceil(bps / (float)cfg.fps) + 1; // add an extra buffer to ensure that there will be no extra audio buffers per frame
    // std::cout<<"bpf "<<bpf<<std::endl;
    waveLen = bpf*buffer_size;
    sample_counter = 0; // sample counter to reach number of samples added in the waveform stack
    wave=new float[waveLen]; // 2 buffers extra to be sure that there will be no extra buffers in between frames
}

void Waveform::prepare_waveform(float *left, float *right){ // exploit right
    
    // std::memcpy(wave,signal,buffer_size*sizeof(double));
    // std::memcpy(wave,signal,buffer_size*sizeof(double));
    
    float energy=0.;
    // if (signal[0]!=0. && signal[buffer_size-1]!=0. && signal[buffer_size/2-1]!=0.){
    for (int i=0; i < buffer_size ; ++i){
        wave[sample_counter++]=left[i];
        energy+=std::pow(left[i],2);
        // std::cout<<"wave["<<i<<"]="<<wave[i]<<" - sample_counter="<<sample_counter<<std::endl;
    }
    // }
    float mean_energy = energy/(float)buffer_size;
    std::cout <<" mean_energy "<< mean_energy<<std::endl;
    if (mean_energy<0.0001){
        sample_counter-=buffer_size; // discards values written in wave by overwritting them in the next audio buffer
    }
    std::cout<<"Waveform filled with signal --> 0:"<<wave[0]<<" 10:"<<wave[10]<<" 511:"<<wave[511]<<std::endl;
}

float* Waveform::getWaveform(float &minf, float &maxf, int &num_samples){

    // int start=currIndex;
    int end;
    
    minf=wave[0];
    maxf=wave[0];

    // for (int i=currIndex;i<buffCount;i++){
    for (int i=0;i<sample_counter;i++){
        if (wave[i]>maxf) maxf=wave[i];
        if (wave[i]<minf) minf=wave[i];
        // std::cout<<"new_i "<<new_i<<" i "<<i<<std::endl;
    }
    num_samples=sample_counter;
    // std::cout<<"sample counter "<<sample_counter<<" is < to "<<bpf<<"??????"<<std::endl;
    

    std::cout<<"waveform min "<<minf<<" max "<<maxf<<std::endl;

    float result[sample_counter];
    std::memcpy(result, wave, (sample_counter)*sizeof(float));

    // TEMP SHOW WAVEFORM

    // if (sample_counter<W){
    //     end=sample_counter;
    // }else end = W;

    // // if (minf!=0 && maxf!=0){
    // int counter=0;
    // for (int x_trans = 0; x_trans < end ; x_trans++ ){
        
        

    //     // double percent = (wave[counter] - min / max - min);
    //     // int ytr = H/2 + (double)H/3*percent;

    //     // normalize in -1 1
    //     float percent = 2* ( (wave[counter]-minf) / (maxf-minf) ) -1;
    //     int ytr = H/2 + 3.0*percent;

    //     // int ytr = (double)H/2. + (double)H*100.*wave[counter];
    //     // std::cout<<" > x,y "<<x_trans<<","<<wave[counter]<<"--> normalized value = "<<ytr<< "with percent = "<<percent<<" , minmax=("<<min<<","<<max<<")"<<std::endl;
    //     // std::cout<<"filling foreground in ("<<x_trans<<","<<ytr<<")"<<std::endl;
    //     tempWaveFrame.at<cv::Vec3b>(ytr,x_trans)[0] = 255;//newval[0];
    //     tempWaveFrame.at<cv::Vec3b>(ytr,x_trans)[1] = 255;//newval[1];
    //     tempWaveFrame.at<cv::Vec3b>(ytr,x_trans)[2] = 255;//newval[2];
    //     counter++;
    // }

    // cv::imshow("Waveform",tempWaveFrame );                       //Showing the video//

    sample_counter=0;

    return wave;

    // // simple downsampling
    // // minf=wave[0];
    // // maxf=wave[0];
    // double result[buffCount-currIndex];
    // std::memcpy(result, wave+currIndex, (buffCount-currIndex)*sizeof(double));
    // // for (int i=currIndex;i<buffCount;i++){

    // //     // if (wave[i]>maxf) maxf=wave[i];
    // //     // if (wave[i]<minf) minf=wave[i];
    // //     // std::cout<<"new_i "<<new_i<<" i "<<i<<std::endl;
    // // }

    // currIndex += buffCount;

    // return result;
}

// void Waveform::get_ring_xy(int &x, int &y){

// }

int Waveform::getWaveLen(){
    return buffers_per_frame*buffer_size;
}