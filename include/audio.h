#ifndef AUDIO_H
#define AUDIO_H

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <jack/jack.h>
#include <jack/types.h>
#include "sine.h"
#include "tone.h"

class Config;
class Waveform;

/*!
// Class for routing audio signal. Uses the jack audio API.
 * @brief A class representing the audio streaming functionality.
 * @note This class manages the audio streaming process, including connecting to the server, creating the connection graph, and streaming the audio buffer.
 * @see the jack library documentation : https://jackaudio.org/api/
 * @see jack client example : https://github.com/jackaudio/example-clients/blob/master/simple_client.c
*/
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
    * @param mutex& - mutex for synchronization with the server
    * @param condition_variable& - condition variable for synchronization with the server
    * @param bool& - boolean indicating whether the server has started
    * @return void
    */
    void clientConnect(std::mutex&, std::condition_variable&, bool&);

    /*! @brief Disconnects the client from the server
    * @return void
    */
    void closeStream();

    /*! @brief Member function for streaming the audio buffer. It is a Callback function called implicitly via the static AudioStream::streamAudio function.
    * Inside the function, the sine wave generator is called to fill the audio buffers with the generated sine waves.
    * @return int - success message
    */
    int streamBuffer();

    /*!
     * @brief Updates the tone member variable with a new frequency and volume.
     * @param int frequency - the current frequency of the tone
     * @param float volume - the volume of the tone
     * @return void
     */
    void update(int,  float);

    /*!
     * @brief Sets the visualizer updater function.
     * @param std::<void(float)> - the visualizer updater function
     * @return void
     * @note The visualizer updater function takes one by one an audio sample as a float parameter to write on the FIFO-based data structures of Waveform and Spectrogram.
     */
    void setVisualizerUpdater(std::function<void(float)>);
    
    /*!
     * @brief Copy constructor is deleted to prevent accidental use.
    */
     AudioStream(const AudioStream&) = delete;
    
    /*!
     * @brief Move constructor is deleted to prevent accidental use.
    */
    AudioStream(AudioStream&&)  = delete;
    
    /*!
     * @brief Copy assignment operator is deleted to prevent accidental use.
    */
    AudioStream& operator=(const AudioStream&) = delete;
    
    /*!
     * @brief Move assignment operator is deleted to prevent accidental use.
    */
    AudioStream& operator=(AudioStream&&) = delete;

private:

    const AudioConfig& audiocfg;
    
    const char *client_name ;
    const char **todevice;
    jack_client_t *client;
    jack_port_t * output_ports[2];
    float *outputBuffers[2];

    Sine sine;
    void (Sine::*make_sound)(Tone&,float*[2]) = nullptr;
    Tone tone;

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





