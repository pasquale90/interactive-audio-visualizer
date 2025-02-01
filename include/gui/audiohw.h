#ifndef AUDIO_HW
#define AUDIO_HW

#include <utility>
#include <vector>

namespace AudioHardware{

    const short int MAX_POTENTIAL_AUDIO_DEVICES = 32;

    const std::vector<unsigned int> supportedRates({8000, 11025, 16000, 22050, 32000, 44100, 48000, 88200, 96000, 176000, 192000, 352800, 384000});
    constexpr int quantizationRatio { sizeof(float) * CHAR_BIT };

    // using AHI=std::vector<std::pair< std::pair<std::string, std::string> , std::pair<unsigned int, unsigned int>>>;

    struct Info{
        std::pair<std::string, std::string> card_info;
        std::pair<unsigned int, unsigned int> sample_rate_range;
        unsigned int numberOfChannels;
    };

    bool get_audio_device_info(int, int, std::pair<unsigned int, unsigned int>&, unsigned int&);

    void get_audio_hardware_info(std::vector<Info>&);

}

#endif