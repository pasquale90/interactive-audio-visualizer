#ifndef SIGNAL_H
#define SIGNAL_H

#include "config.h"
#include <math.h>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif


class Signal{

public:

    /*! @brief Class constructor */
    Signal(int, int);
    
    /*! @brief Default constructor */
    Signal();

    /*! @brief Class destructor.  */
    void erase();

    /*! @brief Class implicit constructor.  */
    void setup(const Config&);

    /*! @brief Method for that generates a phase-shifted tone to fill the current buffer*/
    void prepareSine(int);

    /*! @brief Left phase shiffting method. It is designed for allowing interaction with phase of signal (interacting transition of height) */
    double getSineL();
    
    /*! @brief Right phase shiffting method. It is designed for allowing interaction with phase of signal (interacting transition of height) */
    double getSineR();

private:

    double *sine;
    int sampleRate, bufferSize;

    int prevfreq;

    int phaseL;
    int phaseR;

    double phase;
    double rads_per_sample;
    double amplitude;
};

#endif