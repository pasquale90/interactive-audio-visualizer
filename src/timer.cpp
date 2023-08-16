#include "timer.h"
#include <iostream>
//#include <chrono>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::time_point_cast;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::chrono::seconds;


Timer::Timer(int duration){
    this->ccduration=duration;
    this->remains=duration;
    this->paused=false;
}


void Timer::inverse_count(){
    this->start_point = std::chrono::steady_clock::now();
    this->reach_point = this->start_point + std::chrono::seconds(this->remains);

    auto d=duration_cast<duration<int>>(this->reach_point-this->start_point);

    while(d.count()>0 && !this->paused){
        this->start_point = std::chrono::steady_clock::now();
        d=duration_cast<duration<int>>(this->reach_point-this->start_point);
        this->remains=d.count();
    }
    std::cout<<this->ccduration-this->remains<<" seconds length time period has been elapsed"<<std::endl;
    if (remains>0){
            std::cout<<this->remains <<" seconds remains to complete potato"<<std::endl;
    }
}

void Timer::pause(){
    this->paused=true;
}

void Timer::unpause(){
    this->paused=false;
}

bool Timer::elapsed(){
    if (this->remains>0.0){
        return false;
    }
    else{return true;}
}


void Timer::count_time(){
    if (firstExec){
        now = std::chrono::steady_clock::now();
    }
    
    // std::chrono::steady_clock::time_point then = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    this->start_point = std::chrono::steady_clock::now();
    this->reach_point = this->start_point + std::chrono::seconds(this->remains);

    auto d=duration_cast<duration<int>>(this->reach_point-this->start_point);

    while(d.count()>0 && !this->paused){
        this->start_point = std::chrono::steady_clock::now();
        d=duration_cast<duration<int>>(this->reach_point-this->start_point);
        this->remains=d.count();
    }
    std::cout<<this->ccduration-this->remains<<" seconds length time period has been elapsed"<<std::endl;
    if (remains>0){
            std::cout<<this->remains <<" seconds remains to complete potato"<<std::endl;
    }
}