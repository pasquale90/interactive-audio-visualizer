#ifndef AUDIO_H
#define AUDIO_H

// #include "config.h"
#include <jack/jack.h>
#include <jack/types.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

/*! @brief Callback method - process called when a new audio buffer occurs
  * It forces the Auiolizer and Visualizer classes to provide controlling methods for synchronizing audio (streaming) and image (both capturing & visualization).
  * Two pointers are passed to fill a sine signal using the Audiolizer, which is then passed into the Visualizer for controling image properties using audio analysis tools.
  * @param[out] jack_default_audio_sample_t* left - left sine signal is a pointer to float array for the left channel
  * @param[out] jack_default_audio_sample_t* right - right sine signal is a pointer to float array for the right channel
  * @return void
  */
//@TEMP_COMMENTED
// void audioBufferCallback(jack_default_audio_sample_t* left,jack_default_audio_sample_t* right);

// Class for routing audio signal. Uses the jack audio API.
class AudioStream{
public:

    /*! @brief Class contructor.
    * In this program, a setConfig is used for implicit initialization
    */
    AudioStream(); 

    /*! @brief class destructor
    * disconnects the client from the server
    */
    ~AudioStream();

    /*! @brief Starts a connection to the server and creates the connection graph which connects the inputs with the outputs
    * @return void
    */
    void clientConnect(std::mutex&, std::condition_variable&, bool&);

    /*! @brief Disconnects the client from the server
    * @return void
    */
    void closeStream();

    /*! @brief Method for streaming the audio buffer.
    * Calls the audioBufferCallback to stream pulses of audio data using a float buffer to the rest of the working program.
    * Through the audioBufferCallback, it passes a left and a right buffer as an input to the Audiolizer for filling a buffer of sinuisodal waves of varying frequencies.
    * ... the filled signal is later passed to the Visualizer for audio analysis before visualization, but Visualizer does not change the values of the signal.
    * @param jack_nframes_t nframes - the audio buffer size. Should be replaced with member variable using the Config instance as a parameter
    * @return int - success message
    */
    int streamBuffer(jack_nframes_t nframes);
    
private:
    jack_port_t *input_port;  //@TEMP_IMPL
    jack_port_t *output_port_left;
    jack_port_t *output_port_right;
    jack_client_t *client;

    float *in;
    const char *client_name ;
    const char **fromdevice;  //@TEMP_IMPL
    const char **todevice;

    /*! @brief A non-member fuction used to alias the AudioStream::streamBuffer which is used as an argument in the AudioStream::jack_set_process_callback member function.
    * @param jack_nframes nframes - the buffer size
    * @param void *arg - optional arguments
    */
    static int streamAudio (jack_nframes_t nframes, void *arg);


    /*! @brief Shutdown jack
    */
    static void jack_shutdown (void *arg);
};

#endif





