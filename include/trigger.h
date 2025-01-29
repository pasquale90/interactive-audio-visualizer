#ifndef TRIGGER_H
#define TRIGGER_H

#include "config_types.h"
#include "timer.h"
struct IAVConfig;

constexpr int experience_duration_sec = 10;
constexpr int photo_countdown_sec = 5;

class Trigger {

    public:
        Trigger();

        bool isTrackingEnabled(float&);

        void reset();
        
        Timer* getTimer();
    private:
        IAVConfig& iavcfg;
        Timer timer;

        int timeDurationSec;
        bool mode; // tracking mode or not
        
        void _modeToggle();
};



#endif