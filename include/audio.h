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

  void setup_client(const Config& cfg);

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
    int streamBuffer();

    void update(int,  float);

    void setVisualizerUpdater(std::function<void(float)>);
    
    AudioStream(const AudioStream&) = delete;
    AudioStream(AudioStream&&)  = delete;
    AudioStream& operator=(const AudioStream&) = delete;
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





