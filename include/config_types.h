#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <string>

struct AudioConfig{
    std::string audioDevice;
    int sampleRate;
    int quantization;
    int bufferSize;
};

struct CameraConfig{
    std::string device;
    double frameRate;
    int camResW;
    int camResH;
};

struct DisplayConfig{
    int dispResW;
    int dispResH;
    double fps;
};

struct IAVConfig{
    int minFrequency;
    int maxFrequency;  
    int roiRadius;
    std::string trigger;
    std::string trackingAlg;
};


#endif