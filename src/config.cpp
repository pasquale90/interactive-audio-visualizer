#include "config.h"
#include <iostream>

Config::Config(){
    
    SettingsDB settingsDB;
    settings = settingsDB.loadSettings();

    // Initialize audio configuration
    audconf.audioDevice = settings["audioDevice"];
	audconf.sampleRate.store(std::stoi(settings["sampleRate"])); // --> WILL BE ADJUSTED BY JACK
    audconf.quantization = std::stoi(settings["quantizationRatio"]);
	audconf.bufferSize.store(std::stoi(settings["bufferSize"])); // --> WILL BE ADJUSTED BY JACK

    // Initialize camera configuration
    camconf.device = settings["cameraDevice"];
    std::string camRes = settings["cameraResolution"];
    std::string temp = camRes.substr(0, camRes.find('x')); 
    camconf.camResW.store(std::stoi(temp)); // --> WILL BE ADJUSTED BY OPENCV
    camconf.camResH.store(std::stoi(camRes.substr(temp.length() + 1)));  // --> WILL BE ADJUSTED BY OPENCV
    camconf.frameRate.store(std::stod(settings["cameraFrameRate"]));

    // Initialize screen configuration
    std::string screenRes = settings["displayResolution"];
    temp = screenRes.substr(0, screenRes.find('x'));
    dispconf.dispResW.store(std::stoi(temp));	// --> WILL BE ADJUSTED BY OPENCV
    dispconf.dispResH.store(std::stoi(screenRes.substr(temp.length() + 1)));	// --> WILL BE ADJUSTED BY OPENCV

    // Initialize iav configuration    
    dispconf.fps.store(std::stod(settings["cameraFrameRate"]));
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
    std::cout<<"sampling rate            \t:\t"<<audconf.sampleRate.load()<<" samples/sec"<<std::endl;
    std::cout<<"quantization             \t:\t"<<audconf.quantization<<" bits"<<std::endl;
    std::cout<<"buffer size              \t:\t"<<audconf.bufferSize.load()<<" samples"<<std::endl;
    std::cout<<"----------------- display settings ----------------------\n";
    std::cout<<"frames per second        \t:\t"<<dispconf.fps.load()<<" fps"<<std::endl;
    std::cout<<"display Width            \t:\t"<<dispconf.dispResW.load()<<" pixels"<<std::endl;
    std::cout<<"display Height           \t:\t"<<dispconf.dispResH.load()<<" pixels"<<std::endl;
    std::cout<<"------------------ camera settings ----------------------\n";
    std::cout<<"camera device            \t:\t"<<camconf.device<<std::endl; 
    std::cout<<"camera resolution width  \t:\t"<<camconf.camResW.load()<<" pixels"<<std::endl;
    std::cout<<"camera resolution height \t:\t"<<camconf.camResH.load()<<" pixels"<<std::endl;
    std::cout<<"camera frame rate        \t:\t"<<camconf.frameRate.load()<<" fps"<<std::endl;
    std::cout<<"------------------ iav Settings -------------------------\n";
    std::cout<<"mininum frequency        \t:\t"<<iavconf.minFrequency<<" Hz"<<std::endl;
    std::cout<<"maxinum frequency        \t:\t"<<iavconf.maxFrequency<<" Hz"<<std::endl;
    std::cout<<"radius                   \t:\t"<<iavconf.roiRadius<<" pixels"<<std::endl;
    std::cout<<"trigger method           \t:\t"<<iavconf.trigger<<std::endl;
    std::cout<<"tracking algorithm       \t:\t"<<iavconf.trackingAlg<<std::endl;
    std::cout<<"#########################################################\n\n";

}