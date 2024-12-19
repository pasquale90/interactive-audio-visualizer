#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

/*
* translates the tracking signal into audio frequency
*/

#include <functional>

#include "config_types.h"
struct RegionOfInterest;

class Audiolizer{

public:

   /*! @brief Default constructor.
    */
    Audiolizer();
    
   /*! @brief Member function responsible for obtaining image signal and converting it into sound of a certain frequency.
    * comment method ....
    * @param bool& trackEnabled - variable passed by reference to indicate whether there is a user interaction update.
    * @param int& freq - variable passed by reference that sets a current value to the current generating frequency responce.
    * @param cv::Mat& frame - variable passed by reference that sets a current value to the current generating frequency responce.
    * @param RegionOfInterest& roi - variable passed by reference that updates the value of the current tracking signal.
    * @param[out] float* left - variable passed as a pointer for filling the left channel of the sine wave.
    * @param[out] float* right - variable passed as a pointer for filling the right channel of the sine wave.
    * @return bool - if a new frame has elapsed
    */
    bool turn_Image_into_Sound(const bool, const bool, const RegionOfInterest&, int&);

    void setAudioUpdater(std::function<void(int)>);
private:

    CameraConfig &cameracfg;
    IAVConfig &iavcfg;
    int frequencyRange,prev_freq;

    std::function<void(int)> updateAudio;

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