#ifndef RAW_H
#define RAW_H

#include <vector>
#include <atomic>
#include "config.h"

/*!
 * @brief A circular buffer for storing audio samples.
 * @note The buffer is circular, meaning that when the write position reaches the buffer size, it wraps around to the beginning.
*/
class Waveform{
public:
    
    /*! @brief Default constructor */
    Waveform();

    /*!
     * @brief Writes a new sample to the buffer. Advances the write position by one.
     * @param[in] float sample - the new sample to be written.
     * @return bool - true if the sample was successfully written, false otherwise.
     */
    bool write(const float&);

    /*!
     * @brief Reads a sample from the buffer. Advances the read position by one.
     * @param[out] float& sample - the sample to be read.
     * @return bool - true if the sample was successfully read, false otherwise.
     */
    bool read(float&);
    
    /*!
     * @brief Checks if the buffer is empty.
     * @return bool - true if the buffer is empty, false otherwise. 
    */
    bool isEmpty() const;
    
    /*
     * @brief Checks if the buffer is full.
     * @return bool - true if the buffer is full, false otherwise.
     */
    bool isFull() const;
    
    /*!
     * @brief Returns the capacity of the buffer.
     * @return size_t - the capacity of the buffer.
    */
    size_t size() const;
    
    /*!
     * @brief Returns the number of samples that can be read from the buffer without blocking.
     * @return size_t - the number of samples available for reading.
    */
    size_t availableForReading() const;
    
    /*!
     * @brief Calculates and returns the minimum and maximum values in the buffer.
     * @param[out] float[2] min_max - The minimum and maximum values in the buffer. 0th element is minimum, 1st element is maximum.
     * @return void
    */
    void getMinMax(float[2]);
    
private:

    Config &cfg = Config::getInstance();
    std::vector<float> waveTable; // The actual buffer; 
    std::atomic<size_t> readpos;
    std::atomic<size_t> writepos;
    float min,max;
    size_t capacity;
};

#endif