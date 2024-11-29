#ifndef AUDIO_HW
#define AUDIO_HW

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <utility>
#include <vector>
#include <string>
#include <climits>

const std::vector<unsigned int> supportedRates({8000, 11025, 16000, 22050, 32000, 44100, 48000, 88200, 96000, 176000, 192000, 352800, 384000});
constexpr int quantizationRatio { sizeof(float) * CHAR_BIT };

using AHI=std::vector<std::pair< std::pair<std::string, std::string> , std::pair<unsigned int, unsigned int>>>;

bool get_sampleRate_range(int, int, std::pair<unsigned int, unsigned int>&);

int get_audio_hardware_info(AHI&);

// std::vector<unsigned int> supportedRates;

#endif