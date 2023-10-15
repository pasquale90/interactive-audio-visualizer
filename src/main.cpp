#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
// #include "camera.h"
#include "config.h"
#include "audio.h"
#include "visualizer.h"
#include "beatracker.h"
#include "__audiolizer.h"

// Camera camera;
Beatracker bt;
Visualizer vs;
Audiolizer al;
AudioStream *myAudioStream;

bool exit_msg=false;

int toggleFrame=true;
bool atomicChange;
bool isBeat;
void audioBufferCallback(double* in, int& currenTone){

    // auto t1 = std::chrono::high_resolution_clock::now();


    // get the input from camera --> a signal

    // bool isChanged=al.get_signal(currenTone);
    atomicChange=al.get_signal(currenTone);
    std::cout<<"atomicChange - toggleFrame "<<atomicChange<<" - "<<toggleFrame<<std::endl;

    if (atomicChange!=toggleFrame){
        // process the current input from camera
        std::cout<<"toggle? "<<"Yes!!"<<std::endl;
        toggleFrame=atomicChange;
    }else std::cout<<"toggle? "<<"No :((((((((((((((((((((("<<std::endl;


    isBeat=bt.isBeat(in);
    if (isBeat)
    {
        // do something on the beat
        std::cout<<" Beat! "<<std::endl;
    }
    exit_msg=vs.stream_frames(in,isBeat);


    // atomicChange=camera.get_frame();
    // std::cout<<"atomicChange - toggleFrame "<<atomicChange<<" - "<<toggleFrame<<std::endl;

    // if (atomicChange!=toggleFrame){
    //     // process the current input from camera
    //     std::cout<<"toggle? "<<"Yes!!"<<std::endl;
    //     toggleFrame=atomicChange;
    // }else std::cout<<"toggle? "<<"No :((((((((((((((((((((("<<std::endl;

    std::cout<<"-------------------------------------------------------------------------------------------------------------------"<<std::endl;

//     auto t2 = std::chrono::high_resolution_clock::now();
//     /* Getting number of milliseconds as a double. */
//     std::chrono::duration<double, std::milli> ms_double = t2 - t1;
//     std::cout << ms_double.count() << " ms\n";

    if (exit_msg){
        myAudioStream->~AudioStream();
        // camera.~Camera();
        vs.~Visualizer();
        bt.~Beatracker();
    }
}


int main(int argc,char **argv){

    std::cout<<"\n\n";
    std::cout<<"Hello Audio Visualizer"<<std::endl;
    std::cout<<"\n\n";

    Config cfg(argc,argv);
    cfg.display();

    // camera.setConfig(cfg);
    // camera.display_config();
    
    const char* serverName=NULL;
    const char* clientName="myAudioStream"; 
    myAudioStream = new AudioStream(serverName,clientName);
    // al.setConfig(cfg.bufferSize);
    al.setConfig(cfg);
    bt.setConfig(cfg);
    vs.setConfig(cfg);

    std::thread trackingThread(&Audiolizer::_capture, &al);

    // camera.temp_capture();
    // std::thread thread_obj(&Camera::capture, &camera);
    
    
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

    // thread_obj.join();  
    trackingThread.join();

    // just for testing --> when streaming is discarded in testing phase --> REMOVE LATTER
    // camera.~Camera();
    myAudioStream->~AudioStream();
    vs.~Visualizer();
    bt.~Beatracker();
    cfg.~Config();
    return 0;
}
