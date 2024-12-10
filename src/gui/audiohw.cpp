#include "gui/audiohw.h"
#include <utility>

// using namespace AudioHardware;

bool AudioHardware::get_audio_device_info(int card, int device, std::pair<unsigned int, unsigned int> &sample_rate,unsigned int &numChannels)
{
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    int err;
    char name[32];
    unsigned int sample_rate_min,
                 sample_rate_max;

    // Open the PCM device    
    sprintf(name, "hw:%d,%d", card, device);
    err = snd_pcm_open(&handle, name, SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        // Error opening PCM device
        return false;
    }

    // Allocate hardware parameters object
    snd_pcm_hw_params_alloca(&params);

    // Initialize hwparams with full configuration space
    err = snd_pcm_hw_params_any(handle, params);
    if (err < 0) {
        // Error setting hwparams 
        snd_pcm_close(handle);
        return false;
    }

    // Get sample rate range
    err = snd_pcm_hw_params_get_rate_min(params, &sample_rate_min, nullptr);
    if (err < 0) {
        // Error getting sample rate min 
        snd_pcm_close(handle);
        return false;
    }
    err = snd_pcm_hw_params_get_rate_max(params, &sample_rate_max, nullptr);
    if (err < 0) {
        // Error getting sample rate max
        snd_pcm_close(handle);
        return false;
    }

    sample_rate.first = sample_rate_min;
    sample_rate.second = sample_rate_max;

    // get number of output channels
    err = snd_pcm_hw_params_get_channels(params, &numChannels);// channels now holds the number of channels (outputs)
    if (err < 0 || numChannels ==0 ) {
        // Set the desired number of channels (e.g., 2 for stereo)
        unsigned int atLeastStereo = 2;
        err = snd_pcm_hw_params_set_channels(handle, params, atLeastStereo);
        if (err < 0 || numChannels ==0 ) {
            
            unsigned int atLeastMono = 1;
            err = snd_pcm_hw_params_set_channels(handle, params, atLeastMono);
            if (err < 0 || numChannels ==0) {
                // Error setting channels
                snd_pcm_close(handle);
                return false;
            } 
            // set numChannels to mono
            snd_pcm_hw_params_get_channels(params, &numChannels);
        } else {
            //set numChannels to stereo
            snd_pcm_hw_params_get_channels(params, &numChannels);
        }
    }

    // Close the PCM device
    snd_pcm_close(handle);
    return true;
}

void AudioHardware::get_audio_hardware_info(std::vector<Info> &audio_hw_info){

    int card = -1;
    
    // Loop through all available cards
    while (true) {
        
        // Find the next card
        int err = snd_card_next(&card);
        if (err < 0) {
            // Error getting next card
            break;
        }
        if (card < 0) {
            // No more cards
            break;
        }

        // Open the card control interface
        snd_ctl_t *ctl_handle;
        char ctl_name[32];
        sprintf(ctl_name, "hw:%d", card);
        err = snd_ctl_open(&ctl_handle, ctl_name, 0);
        if (err < 0) {
            // Error opening card
            continue;
        }

        // Get the card info
        snd_ctl_card_info_t *info;
        snd_ctl_card_info_malloc(&info);
        err = snd_ctl_card_info(ctl_handle, info);
        if (err < 0) {
            // Error getting card info
            snd_ctl_close(ctl_handle);
            snd_ctl_card_info_free(info);
            continue;
        }

        std::string card_id = snd_ctl_card_info_get_id(info) ; 
        std::string mixer = snd_ctl_card_info_get_mixername(info);
// card_id = card_id + "("+mixer+")";

        // Check if it is an output device and if it can be opened
        snd_pcm_t *handle;
        std::string card_name_str ="hw:" + std::to_string(card) + ",0"; 
        const char* card_name = card_name_str.c_str();
        if (snd_pcm_open(&handle, card_name, SND_PCM_STREAM_PLAYBACK, 0) >= 0) {
            snd_pcm_close(handle);
        } else {
            continue; // Skip this card if it doesn't support output.
        }

        // Free the card info
        snd_ctl_card_info_free(info);
        snd_ctl_close(ctl_handle);

        // Get PCM device info
        int device = 0;
        unsigned int numChannels = 0;
        std::pair<unsigned int, unsigned int> sample_rate_range;
        while(!AudioHardware::get_audio_device_info(card, device,sample_rate_range,numChannels) && device < MAX_POTENTIAL_AUDIO_DEVICES ){
            ++device;
        }

        Info deviceInfo;
        deviceInfo.card_info = std::make_pair(card_id,mixer);
        deviceInfo.sample_rate_range = sample_rate_range;
        deviceInfo.numberOfChannels = numChannels;
        audio_hw_info.push_back(deviceInfo);
        
    }
}