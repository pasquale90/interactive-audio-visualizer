#ifndef AUDIO_H
#define AUDIO_H

#include <jack/jack.h>
#include <jack/types.h>



#include <iostream>
#include <cstring>
#include <unistd.h>

// int streamAudio (jack_nframes_t nframes, void *arg);

class AudioStream{
public:

    AudioStream(const char* serverName,const char* clientName); //const char*,const char*
    ~AudioStream();

    void AudioRouting();

    
    void closeStream();
    int streamBuffer(jack_nframes_t nframes);

    bool trigger_chunk;

    // int get_signal(float *in,void (*)(float*));

private:
    jack_port_t *input_port;
    jack_port_t *output_port_left;
    jack_port_t *output_port_right;
    jack_client_t *client;

    float *in;
    
    const char *server_name;
    const char *client_name ;
    const char **fromdevice;
    const char **todevice;

    static void jack_shutdown (void *arg);

    // int streamAudio(jack_nframes_t nframes, void *arg); //float *buffer 
};


#endif





