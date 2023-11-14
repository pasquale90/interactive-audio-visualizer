#ifndef AUDIOLIZER_H
#define AUDIOLIZER_H

/*
* here translation of the tracked visual signal into audio sine waves takes place
* recieve data from the tracking.get_data() method and return int frequency
*/

#include "signal.h"
#include "videotracker.h"

class Audiolizer{

public:

   /*! @brief Default constructor.
    */
    Audiolizer();

    /*! @brief Class destructor.
    */
    ~Audiolizer();

   /*! @brief Implicit constructor.
    * @param const Config& cfg - it receives a Config data structure to initialize member variables
    * @return void
    */
    void setConfig(const Config& cfg);
    
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
    bool turn_Image_into_Sound(bool&,int&,cv::Mat&,RegionOfInterest&, float*, float*);
    
   /*! @brief Method responsible for obtaining the camera input in different thread.
    * asynchronous camera frame capturing. It runs in it's own thread.
    * @return void
    */
    void capture();

private:
    /*! @brief Method that calls camera updates
    * @return bool - whether a new camera frame has been captured.
    */
    bool _tickTock();

    /*! @brief Method that receives the tracking updates and handles the way of how the visual interaction will affect sound
    * This function maps the box potition into a certain frequency. Will be updated using more interaction data (i.e. speed)
    * @param[in] RegioOfInterest - detected box stored in struct
    * @param[out] int& - frequency variable passed by reference
    * @returns true if inputs are updated
    */
    bool _translate(RegionOfInterest,int&);

    /*! @brief Method that fills the audio buffer with a sine wave
    * @param[in] int tone - frequency value to fill the current buffer
    * @param[out] float* left - the left audio buffer
    * @param[out] float* right - the right audio buffer
    * @returns void
    */
    void _make_sound(int, float* , float*);
    
    /*! @brief Method for slowly declining frequency responce which is gradualy fading to the value of zero.
    * @param int& freq - the current value of frequency
    * @returns bool - currently not used.
    */
    bool _gradualy_fade(int&);

    Signal sig;
    VideoTracker camera_tracker;
    int bufferSize;
    int prev_freq, minFreq, maxFreq;
    int maxW,maxH;


    /*! @brief Simple method for defining a fixed frequency to a certain value - not used */
    void _simple_definition(int& freq); // defines a fixed frequency
    /*! @brief Simple method for defining a varying frequency between a predifined range of values */
    void _simple_freqRange_palindrome(int& freq);   // defines a moving frequency over time
    bool tempAscenting; // used only for testing methods such as this->_simple_definition() && this->_simple_freqRange_palindrome() 
    int tempFreqcounter;


    /*! @brief Method that is called once during implicit construction to calculate 2 terms (a and b). 
     *a and b are latter used for converting frequency range into a logarithic scale
     *Currently not used.    
     * @param int - the minimum value of the frequency range defined.
     * @param int - the maximum value of the frequency range defined.
     * @returns void
     */
    void _init_log_freq_scale(int,int);

    /*! @brief Method to map the linear values of a predifined frequency range into a logarithmic range.
    * Currently not used.
    * @param int& - frequency variable passed by reference.
    */
    void _int2log_freq(int&);  // currently not used
    double a,b;
};

#endif