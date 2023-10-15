#include "config.h"
#include "audio.h"
#include "camera.h"
#include "visualizer.h"
#include "beatracker.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>

#define WIDTH 1024
#define HEIGHT 512
#define FPS 25
#define CAMWIDHT 160 //320 //640 // //
#define CAMHEIGHT 120 //240 //480 // 240 //
#define CAMFPS 30

// Camera *camera;
// Camera camera(CAMWIDHT,CAMHEIGHT,CAMFPS);
Camera camera;

// std::thread thread_obj2;
// std::atomic<bool> frameToggle;
// bool frameToggle;
Beatracker bt;
Visualizer *vs;

// #include <chrono>


int toggleFrame=true;
void audioBufferCallback(double* in){

    // auto t1 = std::chrono::high_resolution_clock::now();
    
    // updateFrame(in,NULL);
bool isBeat=bt.isBeat(in);
    // std::cout<<"isBeat=bt->isBeat(in);"<<std::endl; 
    
// Onset detection?
    // bool isDownBeat=bt.isDownbeat();
    // bt.getFFT();
    // bool isOnset=bt->isOnset(in);
    
    // std::cout<<"Current tempo estimate "<<bt.getCurrTempoEstimate()<<std::endl;
    if (isBeat)
    {
        // do something on the beat
        std::cout<<" Beat! "<<std::endl;
    }else std::cout<<"Not a Beat .. "<<std::endl;

// vs->stream_frames(in,isBeat);
    std::cout<<"vs->stream_frames(in,isBeat);"<<std::endl; 
    bool current_atomic=camera.get_frame();
    
    std::cout<<"current_atomic - toggleFrame "<<current_atomic<<" - "<<toggleFrame<<std::endl;

    if (current_atomic!=toggleFrame){
        std::cout<<"toggle? "<<"Yes!!"<<std::endl;
        toggleFrame=current_atomic;
    }else std::cout<<"toggle? "<<"No :((((((((((((((((((((("<<std::endl;
    // camera.get_frame(frameToggle);
    
    // camera->get_frame();

    std::cout<<"-------------------------------------------------------------------------------------------------------------------"<<std::endl;

//     auto t2 = std::chrono::high_resolution_clock::now();
//     /* Getting number of milliseconds as a double. */
//     std::chrono::duration<double, std::milli> ms_double = t2 - t1;
//     std::cout << ms_double.count() << " ms\n";
}


int main(int argc,char **argv){
    std::cout<<"memory address of camera before initialization "<<&camera<<std::endl;

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Visualizer"<<std::endl;
    std::cout<<"\n\n";

    int SAMPLE_RATE=std::stoi(argv[1]);
    int BUFFER_SIZE=std::stoi(argv[2]);
    // std::cout<<"SAMPLE RATE = "<<SAMPLE_RATE<<std::endl;
    // std::cout<<"BUFFER_SIZE = "<<BUFFER_SIZE<<std::endl;

    // sampleRate, quantization, bufferSize, fps, dispResX, dispResY, camResX , camResY
    Config cfg(SAMPLE_RATE,24,BUFFER_SIZE,FPS,WIDTH,HEIGHT,CAMWIDHT,CAMHEIGHT,CAMFPS);
    cfg.display();

    // Camera *camera = new Camera(cfg);
    // Camera camera(cfg);
    camera.setConfig(cfg);
    bt.setConfig(cfg);
    std::cout<<"memory address of camera after initialization "<<&camera<<std::endl;

    std::cout<<"CAMERA CONFIG DEBUG TEST ############################################################################################"<<std::endl;
    camera.display_config();
    // camera.temp_capture();
    std::thread thread_obj(&Camera::capture, &camera);
    
    const char* serverName=NULL;
    const char* clientName="myAudioStream"; 
    AudioStream *myAudioStream = new AudioStream(serverName,clientName);

    // bt = new Beatracker(cfg.bufferSize);
    // vs=new Visualizer(WIDTH,HEIGHT,SAMPLE_RATE,BUFFER_SIZE,FPS);    
    // vs=new Visualizer(cfg);

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Stream"<<std::endl;
    std::cout<<"\n\n";
    myAudioStream->AudioRouting();
    // std::thread thread_obj2(&AudioStream::AudioRouting,&myAudioStream);
    myAudioStream->closeStream();

    std::cout<<"\n\n";
    std::cout<<"Reached the end of main"<<std::endl;
    std::cout<<"\n\n";

    

    bt.~Beatracker();
    // vs->~Visualizer();


    thread_obj.join();

    camera.~Camera();
    myAudioStream->~AudioStream();


    return 0;
}
