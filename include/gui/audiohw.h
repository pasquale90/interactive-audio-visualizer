#ifndef AUDIO_HW
#define AUDIO_HW

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <utility>
#include <vector>
#include <string>

using AHI=std::vector<std::pair<std::string, std::pair<unsigned int, unsigned int>>>;

bool get_sampleRate_range(int, int, std::pair<unsigned int, unsigned int>&);

int get_audio_hardware_info(AHI&);

// std::vector<unsigned int> supportedRates;

#endif