#include "waveform.h"
#include <climits>

Waveform::Waveform(): readpos(0), writepos(0){
    // calculate num of buffers per display frame;
    int audioSamplesPerFrame = static_cast<int>(cfg.audconf.sampleRate.load() / cfg.dispconf.fps.load());
    int buffersPerFrame = std::ceil( static_cast<float>(audioSamplesPerFrame) / static_cast<float>(cfg.audconf.bufferSize.load()));
    audioSamplesPerFrame = (buffersPerFrame+1) * cfg.audconf.bufferSize.load(); // add 1 and make it divisible by bufferSize.

    waveTable.reserve(audioSamplesPerFrame);
    waveTable.resize(audioSamplesPerFrame);
    
    min = INT_MAX, max = 0.;
}

bool Waveform::write(const float& arg){ // T&& arg --> rvalue reference
    auto writePos = writepos.load(); //??
    auto nextWritePos = (writePos + 1) % waveTable.size();

    // if the buffer is full, overwrite the oldest data
    if (nextWritePos == readpos.load()) {
        auto readPos = (readpos.load() + 1) % waveTable.size();
        readpos.store(readPos, std::memory_order_release);
    }

    min = std::min(min,arg);
    max = std::max(max,arg);
    
    // write data to the buffer
    waveTable[writePos] = arg;
    writepos.store(nextWritePos);
    return true;

}

bool Waveform::read(float& result){

    auto readPos = readpos.load(); // ????

    // if buffer is empty return false  
    if (readPos == writepos.load()){
        return false;
    }

    // move data out of the buffer;
    result = waveTable[readPos];

    // advance the read pointer
    auto nextReadPos = (readPos + 1) % waveTable.size();
    readpos.store(nextReadPos);
    return true;
}

bool Waveform::isEmpty() const {
    return readpos.load() == writepos.load();
}

bool Waveform::isFull() const {
    return (( writepos.load() + 1 ) % waveTable.size()) == readpos.load();
}

size_t Waveform::size() const { 
    size_t writePos = writepos.load();
    size_t readPos = readpos.load();
    return (writePos - readPos + waveTable.size()) % waveTable.size();
}

size_t Waveform::availableForReading() const {
    size_t writePos = writepos.load();
    size_t readPos = readpos.load();
    if (writePos >= readPos) {
        return writePos - readPos;  // Normal case, no wraparound
    } else {
        return waveTable.size() - (readPos - writePos);  // Wraparound case
    }
}

/**
 * Retrieves the minimum and maximum values from the waveform.
 * @param[out] min The minimum value (output parameter).
 * @param[out] max The maximum value (output parameter).
 */
void Waveform::getMinMax(float minMax[2]){
    minMax[0] = this->min;
    minMax[1] = this->max;
} 
