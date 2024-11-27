#include "gui/audiohw.h"

bool get_sampleRate_range(int card, int device, std::pair<unsigned int, unsigned int> &sample_rate)
{
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    int err;
    char name[32];
    unsigned int sample_rate_min, sample_rate_max;

    // Open the PCM device
    sprintf(name, "hw:%d,%d", card, device);
    err = snd_pcm_open(&handle, name, SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        // Error opening PCM device
        return 0;
    }

    // Allocate hardware parameters object
    snd_pcm_hw_params_alloca(&params);

    // Initialize hwparams with full configuration space
    err = snd_pcm_hw_params_any(handle, params);
    if (err < 0) {
        // Error setting hwparams 
        snd_pcm_close(handle);
        return 0;
    }

    // Get sample rate range
    err = snd_pcm_hw_params_get_rate_min(params, &sample_rate_min, NULL);
    if (err < 0) {
        // Error getting sample rate min 
        snd_pcm_close(handle);
        return 0;
    }
    err = snd_pcm_hw_params_get_rate_max(params, &sample_rate_max, NULL);
    if (err < 0) {
        // Error getting sample rate max
        snd_pcm_close(handle);
        return 0;
    }

    sample_rate.first = sample_rate_min;
    sample_rate.second = sample_rate_max;

    // Close the PCM device
    snd_pcm_close(handle);
    return 1;
}

int get_audio_hardware_info(AHI &audio_hw_info){

    int card = -1;
    int err;

    // Loop through all available cards
    while (true) {
        // Find the next card
        err = snd_card_next(&card);
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
        card_id = card_id + "("+mixer+")";

        // Free the card info
        snd_ctl_card_info_free(info);
        snd_ctl_close(ctl_handle);

         // Get PCM device info
        int device = 0;
        std::pair<unsigned int, unsigned int> sample_rate_range;
        while(!get_sampleRate_range(card, device,sample_rate_range)){
            ++device;
        }
        
        audio_hw_info.push_back({card_id, sample_rate_range});
        
    }

    return EXIT_SUCCESS;
}

// int main(){
//     AHI audio_hw_info;
//     // return get_audio_hardware_info(audio_hw_info);
//     get_audio_hardware_info(audio_hw_info);
//     for (const auto&[name,sr]:audio_hw_info){
//         printf("%s : %d, %d\n",name.c_str(), sr.first, sr.second);
//     }
//     return 0;
// }