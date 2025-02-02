#ifndef TONE_H
#define TONE_H

#include <atomic>

/*!
 * @brief A structure to represent a tone with its frequency and volume.
 * @note The frequency is in Hz, and the volume is in a range from 0.0 to 1.0.
 * @note The default frequency and volume are both set to 0.
 * @see frequency and volume
*/
struct Tone{
    /*! @brief the frequency of the tone in Hz */
    std::atomic<int> frequency {0};
    /*! @brief the volume of the tone, ranging from 0.0 to 1.0 */
    std::atomic<float> volume {0.f};
};


#endif 