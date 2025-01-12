#ifndef TONE_H
#define TONE_H

#include <atomic>

struct Tone{
    std::atomic<int> frequency {0};
    std::atomic<float> volume {0.f};
};


#endif 