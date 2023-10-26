#ifndef SETTINGS_H
#define SETTINGS_H



struct DefaultSettings
{
    //audio
    int sampleRate=44100;
    int quantization=24;
    int bufferSize=512;
    int minFrequency=55;    // A1
    int maxFrequency=3520;  // A7

    //visualizer
    int fps=25;
    int displayW=1024;
    int displayH=512;

    //camera
    int camResW=640;
    int camResH=480;
    int camfps=30;
    int radius=(int)((double)camResH*0.1);  // SIZE OF THE ROI BOX
    int roiSec=5;               // time to wait for capturing pattern of interest within ROI in seconds
    int trackingAlg=0;         // default CSRT, available options = {0:CSRT , 1:KFC, 2:BOOSTING}
//Create checks --> POSTPONED
// #include <list>
// #include <map>
// #include <string>
//     // std::multimap<std::string,list<int>> options_avail 
//     std::map<std::string,std::list<int>> options_avail
//     {
//         {'sample_rate', {8000,16000,22050,44100}},
//         {'bit_quantization', {16,24,32}},
//         {'buffer_size', {32,64,128,256,512,1024,2048,4096}},
//         {'display_frames_per_second', {}}, // change this if other fps values are considered for visualizion
//         {'display_width', {}},
//         {'display_height', {}},
//         {'camera_resolution_width', {}},
//         {'camera_resolution_height', {}},
//         {'camera_frames_per_second', {}},
//         {'roi_waiting_seconds', {}}
//     };
};


#endif