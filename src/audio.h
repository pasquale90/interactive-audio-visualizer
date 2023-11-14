#ifndef AUDIO_H
#define AUDIO_H

// #include "config.h"
#include <jack/jack.h>
#include <jack/types.h>
#include <iostream>
#include <cstring>
#include <unistd.h>


/*! @brief Callback method - process called when a new audio buffer occurs
  * It forces the Auiolizer and Visualizer classes to provide controlling methods for synchronizing audio (streaming) and image (both capturing & visualization).
  * Two pointers are passed to fill a sine signal using the Audiolizer, which is then passed into the Visualizer for controling image properties using audio analysis tools.
  * @param[out] jack_default_audio_sample_t* left - left sine signal is a pointer to float array for the left channel
  * @param[out] jack_default_audio_sample_t* right - right sine signal is a pointer to float array for the right channel
  * @return void
  */
void audioBufferCallback(jack_default_audio_sample_t* left,jack_default_audio_sample_t* right);

/*! @brief A non-member fuction used to alias the AudioStream::streamBuffer which is used as an argument in the AudioStream::jack_set_process_callback member function.
 * @param jack_nframes nframes - the buffer size
 * @param void *arg - optional arguments
 */
int streamAudio (jack_nframes_t nframes, void *arg);

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
    
    /*! @brief implicit contructor
        * Starts a connection to the server
        * @param const char* serverName - name of jack audio server
        * @param const char* clientName - name of the client connected to the server
        * @return void
    */
    void setConfig(const char* serverName,const char* clientName); 

    /*! @brief Creates the connection graph which connects the inputs with the outputs
    * @return void
    */
    void AudioRouting();

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
    jack_port_t *input_port;
    jack_port_t *output_port_left;
    jack_port_t *output_port_right;
    jack_client_t *client;

    double *in;
    const char *server_name;
    const char *client_name ;
    const char **fromdevice;
    const char **todevice;

    /*! @brief Shutdown jack
    */
    static void jack_shutdown (void *arg);

    /*! @todo mix multiple input sources
     *  function to mix (inplace) the left and right outs with all the input signals received from all the input channels supported by the audio device
     *  nores :for a more generic app, that makes use of the input sig from a USB audio interface or the OS sys ...
     *  .. we ought to combine the two signals : the input from the channel(s) and the sine tone.
     */
    void mix();
};

#endif





