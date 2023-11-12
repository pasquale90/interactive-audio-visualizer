#ifndef CONFIG_H
#define CONFIG_H

#include "console.h"
#include "config_defaults.cpp"

struct Config{
    int sampleRate;
    int quantization;
    int bufferSize;
    
    int minFrequency;
    int maxFrequency;

    int fps;
    int displayW;
    int displayH;
    
    int camResW;
    int camResH;
    int camfps;
    
    int radius;
    int roiSec;

    int trackingAlg;

    Config(int argc, char *argv[]);
    Config(); // default constructor
    Config(const Config& c); // copy constructor
    ~Config();
    
    void display();
};

#endif