#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>

struct Config{
    int sampleRate;
    int quantization;
    int bufferSize;
    
    int fps;
    int displayW;
    int displayH;
    
    int camResW;
    int camResH;
    int camfps;
    
    Config(int sr,int qnt, int bfs, int fs, int dresx, int dresy, int cresx, int cresy, int cfs);
    Config(); // default constructor
    Config(const Config& c); // copy constructor
    
    void display();
};

#endif