#ifndef RAW_H
#define RAW_H

#include "config.h"
#include <math.h>

#include <atomic>
#include <vector>
#include <iostream>

class Waveform{
public:
    
    /*! @brief Default constructor */
    Waveform();

    bool write(const float&);
    bool read(float&);
    
    bool isEmpty() const;
    bool isFull() const;
    size_t size() const;
    size_t availableForReading() const;
    void getMinMax(float[2]);
    
private:

    Config &cfg = Config::getInstance();
    std::vector<float> waveTable; // The actual buffer; 
    std::atomic<size_t> readpos;
    std::atomic<size_t> writepos;
    float min,max;
};

#endif