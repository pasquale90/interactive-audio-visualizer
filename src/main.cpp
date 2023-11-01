#include <cstdlib>
#include <chrono>
#include <thread>

#include "audio.h"
#include "visualizer.h"
#include "beatracker.h"
#include "audiolizer.h"

Beatracker bt;
Visualizer vs;
Audiolizer al;
AudioStream *myAudioStream;

RegionOfInterest ROI;
cv::Mat visualFrame;
bool exit_msg=false;
bool isBeat;
bool trackEnabled;

void audioBufferCallback(double* mix, int& currenTone){
    /*** fill latter
     * frameElapsed : a new frame has been elapsed (enables visualization)
     * trackEnabled : ROI's tracking signal (updates visualization)
     * currenTone : audiolizer's generated frequency responce                                   // currently controls the color changing --> in other project, the mix should do it.
     * visualFrame : the whole frame of the camera when passed through ```turn_Image_into_Sound()``` and the visual output when passed through ```and_Sound_into_Image()``` 
     * ROI : region of interest.Center(x,y), and volume(width, height) 
     * // @TODO is to 
    */
    auto t1 = std::chrono::high_resolution_clock::now();

    // get the input from camera --> a signal
    bool frameElapsed=al.turn_Image_into_Sound(trackEnabled,currenTone,visualFrame,ROI);

    // std::cout<<"MAIN :: Current Roi center "<<ROI.centerX<<","<<ROI.centerY<<std::endl;
    if (frameElapsed){
        std::cout<<"frameElapsed toggle? "<<"Yes!!"<<std::endl;
    }else std::cout<<"toggle? "<<"No :((((((((((((((((((((("<<std::endl;

    // isBeat=bt.isBeat(in);
    // if (isBeat)
    // {
    //     // do something on the beat
    //     std::cout<<" Beat! "<<std::endl;
    // }

    exit_msg=vs.and_Sound_into_Image(mix,visualFrame,frameElapsed,trackEnabled,currenTone);

    std::cout<<"-------------------------------------------------------------------------------------------------------------------"<<std::endl;

    auto t2 = std::chrono::high_resolution_clock::now();
    /* Getting number of milliseconds as a double. */
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << " ms\n";

    if (exit_msg){
        myAudioStream->~AudioStream(); // check if jackshutdown is set appropriately
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
    
    const char* serverName=NULL;
    const char* clientName="myAudioStream"; 
    myAudioStream = new AudioStream(cfg,serverName,clientName);

    al.setConfig(cfg);
    bt.setConfig(cfg);
    vs.setConfig(cfg);

    std::thread trackingThread(&Audiolizer::_capture, &al);
        
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
