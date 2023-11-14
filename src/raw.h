#ifndef RAW_H
#define RAW_H

#include "config.h"
#include <iostream>
#include <math.h>
#include <cstring>

class Waveform{
public:
    
    /*! @brief Default constructor */
    Waveform();

    /*! @brief Class destructor */
    ~Waveform();    

    /*! @brief Implicit constructor */
    void set_config(const Config&);

    /*! @brief It fills an unknown size (buffer_size * (?)BuffersElapsed) of data obtained from each buffer, while next visualization frame is not currently changing */
    void prepare_waveform(float*,float*);

    /*! @brief It returns data collected from all buffers occured on the meantime, and sends them all to the Visualizer for further processing and visualization */
    float* getWaveform(float&, float&, int&);

    /*! @brief Returns the current length of the signal */
    int getWaveLen();
    
private:
    float *wave;
    int waveLen;
    int buffer_size; 
    int sample_counter;
    
};

#endif