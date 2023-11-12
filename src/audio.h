#ifndef AUDIO_H
#define AUDIO_H

#include "config.h"
#include <jack/jack.h>
#include <jack/types.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

class AudioStream{
public:

    AudioStream(const Config&, const char*,const char*); //const char*,const char*
    ~AudioStream();

    void AudioRouting();

    void closeStream();
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

    static void jack_shutdown (void *arg);

    // make a mix() function to mix (inplace) the left and right outs with all the input signals received from all the input channels supported by the audio device
};


#endif





