#include <iostream>
#include "timer.h"

Timer::Timer() : initialSeconds(0), remainingTimeMilliseconds(0){}

void Timer::setTimer(int seconds) {
    initialSeconds = seconds;  
    remainingTimeMilliseconds = seconds * 1000; 
    lastUpdateTime = std::chrono::steady_clock::now();
}

void Timer::start(){
    lastUpdateTime = std::chrono::steady_clock::now(); 
}

bool Timer::getCurrentTime(int& secondsElapsed) {

    // Calculate time passed since last update
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<int64_t, std::milli> elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);

    // Subtract elapsed milliseconds from the remaining time
    remainingTimeMilliseconds -= static_cast<int>(elapsed.count());  
    lastUpdateTime = now;  // Update last time frame

    // If the timer has finished
    if (remainingTimeMilliseconds <= 0) {
        remainingTimeMilliseconds = 0;
        secondsElapsed = initialSeconds;  // Set elapsed time to the initial value
        return true; 
    }

    // Return the elapsed time in seconds (rounded)
    secondsElapsed = remainingTimeMilliseconds;
    return false;
}

// Function to get the remaining time for display or logging purposes
int Timer::getRemainingTime() const {
    return remainingTimeMilliseconds / 1000;  // Convert milliseconds back to seconds
}

// Function to check if the timer has finished
bool Timer::isTimerFinished() const {
    return remainingTimeMilliseconds <= 0;
}

void Timer::pauseTimer() {
    remainingTimeMilliseconds = 1;
}
