#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

#include <functional>
#include "config_types.h"
struct RegionOfInterest;
class Tone;

/*!
 * @brief A class responsible for translating tracking signal into audio frequency.
 * @note This class is responsible for managing the conversion from image to audio.
 */
class Audiolizer{

public:

   /*! @brief Default constructor.
    */
    Audiolizer();
    
   /*! @brief Member function responsible for obtaining image signal and converting it into sound of a certain frequency.
    * @param[in] bool trackingUpdated - variable that indicates whether there is a new tracking signal.
    * @param[in] bool trackingEnabled - variable that indicates whether the tracking is enabled or not.
    * @param RegionOfInterest& roi - variable passed by reference that updates the value of the current tracking signal.
    * @param[in] Tone& - the tone object for storing the current frequency and volume
    * @return bool - returns true if frequency has changed
    */
    bool turn_Image_into_Sound(const bool, const bool, const RegionOfInterest&, Tone&);

    /**
     * @brief Method that sets the audio updater function.
     * @param std::function<void(int,float)> - the audio updater function that receives as parameters the current frequency and volume.
     * @return void
     */
    
    void setAudioUpdater(std::function<void(int,float)>);

private:

    CameraConfig &cameracfg;
    IAVConfig &iavcfg;
    int frequencyRange,prev_freq;
    float volume;

    std::function<void(int,float)> updateAudio;

    /*! @brief Method that receives the tracking updates and handles the way of how the visual interaction will affect sound
    * This function maps the box potition into a certain frequency. Will be updated using more interaction data (i.e. speed)
    * @param[in] RegioOfInterest - detected box stored in struct
    * @param[out] int& - frequency variable passed by reference
    * @returns true if inputs are updated
    */
    bool translate(const RegionOfInterest&,int&);
    
    /*! @brief Method for slowly declining frequency responce which is gradualy fading to the value of zero.
    * @param int& freq - the current value of frequency
    * @returns bool - currently not used.
    */
    void gradualy_fade(int&);

    // @TEMPORARY DISABLED
    // /*! @brief Method that is called once during implicit construction to calculate 2 terms (a and b). 
    //  *a and b are latter used for converting frequency range into a logarithic scale
    //  *Currently not used.    
    //  * @param int - the minimum value of the frequency range defined.
    //  * @param int - the maximum value of the frequency range defined.
    //  * @returns void
    //  */
    // void init_log_freq_scale();

    // /*! @brief Method to map the linear values of a predifined frequency range into a logarithmic range.
    // * Currently not used.
    // * @param int& - frequency variable passed by reference.
    // */
    // void int2log_freq(int&);  // currently not used
    // double a,b;
};

#endif