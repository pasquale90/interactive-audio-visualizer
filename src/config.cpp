#include "config.h"
#include <iostream>

Config::Config(){
    
    SettingsDB settingsDB;
    settings = settingsDB.loadSettings();

    // Initialize audio configuration
    audconf.audioDevice = settings["audioDevice"];
    audconf.sampleRate = std::stoi(settings["sampleRate"]); // --> WILL BE ADJUSTED BY JACK
    audconf.quantization = std::stoi(settings["quantizationRatio"]);
    audconf.bufferSize = std::stoi(settings["bufferSize"]); // --> WILL BE ADJUSTED BY JACK

    // Initialize camera configuration
    camconf.device = settings["cameraDevice"];
    std::string camRes = settings["cameraResolution"];
    std::string temp = camRes.substr(0, camRes.find('x'));
    camconf.camResW = std::stoi(temp);
    camconf.camResH = std::stoi(camRes.substr(temp.length() + 1));
    camconf.frameRate = std::stod(settings["cameraFrameRate"]);

    // Initialize screen configuration
    std::string screenRes = settings["displayResolution"];
    temp = screenRes.substr(0, screenRes.find('x'));
    dispconf.dispResW = std::stoi(temp);
    dispconf.dispResH = std::stoi(screenRes.substr(temp.length() + 1));

    // Initialize iav configuration    
    dispconf.fps = std::stod(settings["cameraFrameRate"]);
    if (settings["frequencyRange"] == "Narrow")
    {
            iavconf.minFrequency = 300;
            iavconf.maxFrequency = 700;
    }
    else if (settings["frequencyRange"] == "Normal")
    {
            iavconf.minFrequency = 300;
            iavconf.maxFrequency = 1500;
    }
    else if (settings["frequencyRange"] == "Wide")
    {
            iavconf.minFrequency = 100;
            iavconf.maxFrequency = 3000;
    } 
    
    if (settings["roi"] == "Small")
            iavconf.roiRadius = 0.05 * camconf.camResW;
    else if (settings["roi"] == "Medium")
            iavconf.roiRadius = 0.1 * camconf.camResW;
    else if (settings["roi"] == "Large")
            iavconf.roiRadius = 0.15 * camconf.camResW;
    iavconf.trigger = settings["trigger"];
    iavconf.trackingAlg = settings["trackingAlgorithm"];
}

void Config::display(){

    std::cout<<"########## Interactive Audio Visualizer Config ##########\n";
    std::cout<<"------------------ audio settings -----------------------\n";
    std::cout<<"audio device             \t:\t"<<audconf.audioDevice<<std::endl;
    std::cout<<"sampling rate            \t:\t"<<audconf.sampleRate<<" samples/sec"<<std::endl;
    std::cout<<"quantization             \t:\t"<<audconf.quantization<<" bits"<<std::endl;
    std::cout<<"buffer size              \t:\t"<<audconf.bufferSize<<" samples"<<std::endl;
    std::cout<<"----------------- display settings ----------------------\n";
    std::cout<<"frames per second        \t:\t"<<dispconf.fps<<" fps"<<std::endl;
    std::cout<<"display Width            \t:\t"<<dispconf.dispResW<<" pixels"<<std::endl;
    std::cout<<"display Height           \t:\t"<<dispconf.dispResH<<" pixels"<<std::endl;
    std::cout<<"------------------ camera settings ----------------------\n";
    std::cout<<"camera device            \t:\t"<<camconf.device<<std::endl; 
    std::cout<<"camera resolution width  \t:\t"<<camconf.camResW<<" pixels"<<std::endl;
    std::cout<<"camera resolution height \t:\t"<<camconf.camResH<<" pixels"<<std::endl;
    std::cout<<"camera frame rate        \t:\t"<<camconf.frameRate<<" fps"<<std::endl;
    std::cout<<"------------------ iav Settings -------------------------\n";
    std::cout<<"mininum frequency        \t:\t"<<iavconf.minFrequency<<" Hz"<<std::endl;
    std::cout<<"maxinum frequency        \t:\t"<<iavconf.maxFrequency<<" Hz"<<std::endl;
    std::cout<<"radius                   \t:\t"<<iavconf.roiRadius<<" pixels"<<std::endl;
    std::cout<<"trigger method           \t:\t"<<iavconf.trigger<<std::endl;
    std::cout<<"tracking algorithm       \t:\t"<<iavconf.trackingAlg<<std::endl;
    std::cout<<"#########################################################\n\n";

}