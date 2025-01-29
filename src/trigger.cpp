#include "trigger.h"
#include "config.h"

Trigger::Trigger() : iavcfg (Config::getInstance().iavconf),timeDurationSec(photo_countdown_sec),mode(0)
{

    timer.setTimer(timeDurationSec);
    
    // if (iavcfg.trigger == "Auto"){} 
    // else if (iavcfg.trigger == "Manual"){}
}

void Trigger::_modeToggle(){
    mode =!mode;
    timeDurationSec = (mode) ? experience_duration_sec : photo_countdown_sec;
}

bool Trigger::isTrackingEnabled(float &remaining_percentage){


        if (timer.isTimerFinished()){
            _modeToggle();
            timer.setTimer(timeDurationSec);
        }
        
        int millisecondsElapsed;
        
        timer.getCurrentTime(millisecondsElapsed);
        remaining_percentage = (static_cast<float> (millisecondsElapsed) / static_cast<float>(timeDurationSec*1000)); 
        
        return mode;
}

void Trigger::reset(){
    mode = 0;
    timeDurationSec = photo_countdown_sec;
}

Timer* Trigger::getTimer(){
    return &timer;
}