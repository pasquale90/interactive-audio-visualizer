#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>

struct DefaultSettings
{
    //audio
    int sampleRate=44100;
    int quantization=24;
    int bufferSize=512;

    //visualizer
    int fps=25;
    int displayW=1024;
    int displayH=512;

    //camera
    int camResW=640;
    int camResH=480;
    int camfps=30;
    int roiOffset=camResH*0.1;  // SIZE OF THE ROI BOX
    int roiSec=5;               // time to wait for capturing pattern of interest within ROI in seconds

};


#endif