#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

class Timer {

    public:
        Timer();

        void setTimer(int);

        void start();

        bool getCurrentTime(int&);

        int getRemainingTime() const;

        bool isTimerFinished() const;

        void pauseTimer();
        
    private:

        int initialSeconds;                     // The initial time to start the countdown from (in seconds)
        int remainingTimeMilliseconds;          // Remaining time in milliseconds
        std::chrono::steady_clock::time_point lastUpdateTime;  // Last frame timestamp to calculate elapsed time

};



#endif