#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <string>
#include <atomic>

struct AudioConfig{
    std::string audioDevice;
    std::atomic<int> sampleRate;
    int quantization;
    std::atomic<int> bufferSize;
};

struct CameraConfig{
    std::string device;
    std::atomic<double> frameRate;
    std::atomic<int> camResW;
    std::atomic<int> camResH;
};

struct DisplayConfig{
    std::atomic<int> dispResW;
    std::atomic<int> dispResH;
    std::atomic<double> fps;
};

struct IAVConfig{
    int minFrequency;
    int maxFrequency;  
    int roiRadius;
    std::string trigger;
    std::string trackingAlg;
};


#endif