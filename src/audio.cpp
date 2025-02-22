#include <cstddef>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "audio.h"
#include "unused_defines.h"
#include "config.h"


const char *server_name = nullptr;
const char clientName[17] = "IAV-audio-client";

int AudioStream::streamAudio ( jack_nframes_t UNUSED(nframes), void *arg){ //, float *in,void (*threading)(float *sig)
    
    return static_cast<AudioStream*>(arg)->streamBuffer();
}

AudioStream::AudioStream():audiocfg (Config::getInstance().audconf){

    client_name=clientName;
    // nullify all
    client = nullptr;
    todevice = nullptr;
    
    if (audiocfg.numChannels.load() == 1){
        output_ports[1] = nullptr;
        outputBuffers[1]=nullptr;
        make_sound = &Sine::setMonoSignal;  // Point to setMonoSignal for processing 1 single mono buffer
    }        
    else if (audiocfg.numChannels.load() == 2) {
        make_sound = &Sine::setStereoSignal;  // Point to setStereoSignal for processing 2 stereo buffers
    }
}

void AudioStream::setVisualizerUpdater(std::function<void(float)> updater){
    sine.setVisualizerUpdater(std::move(updater));
}

AudioStream::~AudioStream(){
    closeStream();
}

void AudioStream::clientConnect(std::mutex& mtx, std::condition_variable& cv, bool& serverStarted){

    // Waiting for jack server to start
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] { return serverStarted; });

    jack_options_t options = JackNoStartServer;//(JackSessionID|JackServerName|JackNoStartServer|JackUseExactName|JackNullOption)
	jack_status_t status;

    /* open a client connection to the JACK server */
	client = jack_client_open (client_name, options, &status,nullptr);
    if (status & JackNameNotUnique) {    //client name not unique, set a client name;
        client_name = jack_get_client_name(client);
        std::cerr<<"\t>>unique name "<<client_name<<" assigned to the client obj."<<std::endl;;
    }
    
	if (client == NULL) {
        std::cerr<<"\t>>jack_client_open() failed, status = "<<status<<std::endl;
        if (status & JackServerFailed) {
            std::cerr<<"\t>>Unable to connect to JACK server"<<std::endl;
        }
        exit (1);
    }
    if (status & JackServerStarted) {
        printf("JACK server started\n");
    }

    //callback
    if (jack_set_process_callback (client,streamAudio,this)){ //arg
            std::cerr<<"\t>>Callback operation failed"<<std::endl;
    }

    //prevent failure
    jack_on_shutdown(client,&jack_shutdown,0);

    //register physical ports
    for (size_t ch=0; ch<audiocfg.numChannels.load();++ch){
        std::string portName = (ch%2) ? ("PortRight"+std::to_string(ch/2)) : ("PortLeft"+std::to_string(ch/2));
        output_ports[ch]=jack_port_register (client,portName.c_str(),JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        if (output_ports[ch] == NULL){
            std::cerr<<"\t>>Unable to register output port for {"<<jack_port_name(output_ports[ch])<<"}"<<std::endl;;
            exit (1);}
    }
    
    //activate client
    if (jack_activate (client)) {
        std::cerr<<"\t>>cannot activate client {"<<client_name<<"}"<<std::endl;
        exit (1);
    }

    // Getting acces to destination ports
    todevice = jack_get_ports (client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if (todevice == NULL) {
        std::cerr<<"\t>>no physical playback devices"<<std::endl;
        exit (1);
    }

    for (size_t ch=0; ch <audiocfg.numChannels.load();++ch){
        if (output_ports[ch]!=NULL){
            if (jack_connect (client, jack_port_name(output_ports[ch]), todevice[ch])){//returns full name
                std::cerr<<"\t>>cannot connect left plysical output port {"<<todevice[ch]<<"} with input port {"<<jack_port_name(output_ports[ch])<<"}"<<std::endl;
            }
        }
    }

    free (todevice);
}


void AudioStream::closeStream(){
    
    for (size_t i=0; i<audiocfg.numChannels.load();++i){
        if (jack_port_connected(output_ports[i])){
            if(jack_port_disconnect(client,output_ports[i])){
                std::cerr<<"Couldnt disconnect the "<<jack_port_name(output_ports[i])<<" output port from the main stream\n";
            }
        }
        
    }
    jack_client_close (client);
}

int AudioStream::streamBuffer(){

    for (size_t ch = 0 ; ch < audiocfg.numChannels.load(); ++ch){
        outputBuffers[ch] = static_cast<float *>(jack_port_get_buffer (output_ports[ch], audiocfg.bufferSize.load() ));
    }
    
    (sine.*make_sound)(tone,outputBuffers); 
    

    return 0;
}

void AudioStream::jack_shutdown (void *UNUSED(arg))
{
	exit (1);
}

/**
 * Retrieves the minimum and maximum values from the waveform.
 * @param[out] frequency The tones spectral frequency.
 * @param[out] volume The volume of the tone.
 */
void AudioStream::update(int frequency, float volume){
    tone.frequency.store(frequency);
    tone.volume.store(volume);
}
