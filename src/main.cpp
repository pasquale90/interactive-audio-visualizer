/*!*******************************************//**
 * @file
 * @author Melissas Paschalis <melissaspaschalis@gmail.com>
 * @version 1.0 - demo
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
***********************************************/

#include <cstdlib>
#include <chrono>
#include <thread>
#include "audio.h"
#include "visualizer.h"
#include "audiolizer.h"
// #include "beatracker.h"

// Beatracker bt;
Visualizer vs;
Audiolizer al;
AudioStream myAudioStream;

RegionOfInterest ROI;
cv::Mat visualFrame;
bool exit_msg=false;
bool isBeat;
bool trackEnabled;
int currenTone;

void audioBufferCallback(jack_default_audio_sample_t* left, jack_default_audio_sample_t *right){ // --> jack_default_audio_sample_t* === float*
    /*** fill latter
     * frameElapsed : a new frame has been elapsed (enables visualization)
     * trackEnabled : ROI's tracking signal (updates visualization)
     * currenTone : audiolizer's generated frequency responce                                   // currently controls the color changing --> in other project, the mix should do it.
     * visualFrame : the whole frame of the camera when passed through ```turn_Image_into_Sound()``` and the visual output when passed through ```and_Sound_into_Image()``` 
     * ROI : region of interest.Center(x,y), and volume(width, height) 
     * // @TODO is to 
    */
    
// auto t1 = std::chrono::high_resolution_clock::now();

    // get the input from camera --> a signal
    bool frameElapsed=al.turn_Image_into_Sound(trackEnabled,currenTone,visualFrame,ROI, (float *)left ,  (float *)right); // rename frameElapsed with the var name "imClock"

    // if (bt.isBeat((double *)in))
    //     std::cout<<" Beat! "<<std::endl;      // do something on the beat
    
    exit_msg=vs.and_Sound_into_Image((float *)left, (float *)right, visualFrame, frameElapsed, trackEnabled, currenTone, ROI);

/* Getting number of milliseconds as a double. */
// auto t2 = std::chrono::high_resolution_clock::now();
// std::chrono::duration<double, std::milli> ms_double = t2 - t1;
// std::cout << ms_double.count() << " ms\n";

    // std::cout<<"----------------------------------------------------------------------------------------------------------------------------------------------------------"<<std::endl;
    
    if (exit_msg){
        myAudioStream.~AudioStream(); // check if jackshutdown is set appropriately
        vs.shutdown();
        al.shutdown();
        // bt.~Beatracker();
    }
}

/*! @brief main function.
  * Two pointers are passed to fill a sine signal using the Audiolizer, which is then passed into the Visualizer for controling image properties using audio analysis tools.
  * @param int argc - number of input arguments
  * @param char **argv - arguments. These are parsed through the InputParser class defined in console.* files.
  * @return int - success / failure of program
  */
int main(int argc,char **argv){

    Config cfg(argc,argv);
    cfg.display();
    
    const char* serverName=NULL;
    const char* clientName="myAudioStream"; 
    // myAudioStream = new AudioStream(cfg,serverName,clientName);
    myAudioStream.setup(serverName,clientName);
    al.setup(cfg);
    // bt.setConfig(cfg);
    vs.setup(cfg);

    std::thread trackingThread(&Audiolizer::capture, &al);

    trackingThread.join();

    return 0;
}
