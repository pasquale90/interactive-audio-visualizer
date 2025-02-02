#ifndef AUDIO_HW
#define AUDIO_HW

#include <utility>
#include <vector>

/*!
 * @brief Audio hardware namespace provides functions to interact with audio hardware devices.
 * @note This namespace encapsulates the functionality to retrieve information about available audio devices, sample rates, and number of channels.
*/
namespace AudioHardware{

    /*!
    * @brief Custom defined - maximum number of potential audio devices that can be retrieved.
    */
    const short int MAX_POTENTIAL_AUDIO_DEVICES = 32;

    /*!
    * @brief List of supported sample rates.
    * @see for detailed info see : https://en.wikipedia.org/wiki/Sampling_(signal_processing)#Audio_sampling
    */
    const std::vector<unsigned int> supportedRates({8000, 11025, 16000, 22050, 32000, 44100, 48000, 88200, 96000, 176000, 192000, 352800, 384000});
    
    /*!
     * @brief Bit size of the floating-point samples in bits.
    */
    constexpr int quantizationRatio { sizeof(float) * CHAR_BIT };

    // using AHI=std::vector<std::pair< std::pair<std::string, std::string> , std::pair<unsigned int, unsigned int>>>;

    /*!
     * @brief Structure representing audio hardware information
     * @note This structure encapsulates the information about audio hardware devices and their supported features.
     * @see std::pair<std::string, std::string> card_info - audio card index and audio mixer index, coupled.
     * @see sample_rate_range - supportedRates for the list of supported sample rates.
     * @see numberOfChannels - the number of output audio device channels
    */
    struct Info{
        std::pair<std::string, std::string> card_info;
        std::pair<unsigned int, unsigned int> sample_rate_range;
        unsigned int numberOfChannels;
    };

    /*!
     * @brief Retrieves information about available audio devices related to a specific audio card
     * @param int card - audio card index
     * @param int device - audio device index
     * @param std::pair<unsigned int, unsigned int> &sample_rate - sample rate range (min, max)
     * @param unsigned int &numChannels - number of audio channels
     * @return bool - true if information related to the specific audio card is successfully retrieved
    */
    bool get_audio_device_info(int, int, std::pair<unsigned int, unsigned int>&, unsigned int&);

    /*!
     * @brief Retrieves information about available audio cards and their supported audio devices
     *
     * This function scans the system for connected audio devices and gathers information
     * about each one of them, including its name id, the sample rate range supported and the number of output channels.
     *
     * @param std::vector<Info> &audio_hw_info - vector to store audio hardware information
     * @return void
    */
    void get_audio_hardware_info(std::vector<Info>&);

}

#endif