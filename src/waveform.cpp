#include "waveform.h"



#include <climits> //erase all these
#include <limits>
#include <algorithm>

Waveform::Waveform(): readpos(0), writepos(0){
    // calculate num of buffers per display frame;
    int audioSamplesPerFrame = static_cast<int>(cfg.audconf.sampleRate / cfg.dispconf.fps);
    audioSamplesPerFrame*=2; // make them double.
    
    waveTable.reserve(audioSamplesPerFrame);
    waveTable.resize(audioSamplesPerFrame);
    
    // std::cout<<"wavetable --->"<<waveTable.size()<<" "<<waveTable.capacity()<<std::endl;
    // buffersPerFrame = std::ceil( static_cast<float>(cfg.audconf.sampleRate) / static_cast<float>(cfg.dispconf.fps * cfg.audconf.bufferSize) );
    // std::cout<<buffersPerFrame

    bufferCount = 0; 
    min = 10., max = 0.;
}

bool Waveform::write(const float& arg){ // T&& arg --> rvalue reference
    auto writePos = writepos.load(std::memory_order_relaxed); //??
    auto nextWritePos = (writePos + 1) % waveTable.size();

    // if the buffer is full, overwrite the oldest data
    if (nextWritePos == readpos.load(std::memory_order_acquire)) {
        auto readPos = (readpos.load(std::memory_order_relaxed) + 1) % waveTable.size();
        readpos.store(readPos, std::memory_order_release);
    }

min = std::min(min,arg);
max = std::max(max,arg);
    
    // write data to the buffer
    waveTable[writePos] = arg;
    writepos.store(nextWritePos, std::memory_order_release);
    return true;

}

bool Waveform::read(float& result){

    auto readPos = readpos.load(std::memory_order_relaxed); // ????

    // if buffer is empty return false  
    if (readPos == writepos.load(std::memory_order_acquire)){
        return false;
    }

    // move data out of the buffer;
    result = waveTable[readPos];

    // advance the read pointer
    auto nextReadPos = (readPos + 1) % waveTable.size();
    readpos.store(nextReadPos, std::memory_order_release);
    return true;
}

bool Waveform::isEmpty() const {
    return readpos.load(std::memory_order_acquire) == writepos.load(std::memory_order_acquire);
}

bool Waveform::isFull() const {
    return (( writepos.load(std::memory_order_acquire) + 1 ) % waveTable.size()) == readpos.load(std::memory_order_acquire);
}

size_t Waveform::size() const { 
    size_t writePos = writepos.load(std::memory_order_acquire);
    size_t readPos = readpos.load(std::memory_order_acquire);
    return (writePos - readPos + waveTable.size()) % waveTable.size();
}

size_t Waveform::availableForReading() const {
    size_t writePos = writepos.load(std::memory_order_acquire);
    size_t readPos = readpos.load(std::memory_order_acquire);
    if (writePos >= readPos) {
        return writePos - readPos;  // Normal case, no wraparound
    } else {
        return waveTable.size() - (readPos - writePos);  // Wraparound case
    }
}
