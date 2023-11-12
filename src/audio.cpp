#include "audio.h"

void audioBufferCallback(jack_default_audio_sample_t*,jack_default_audio_sample_t*);

int streamAudio (jack_nframes_t nframes, void *arg){ //, float *in,void (*threading)(float *sig)
    return static_cast<AudioStream*>(arg)->streamBuffer(nframes);
}
AudioStream::AudioStream(const Config &cfg, const char* serverName,const char* clientName){
    
    server_name=serverName;
    client_name=clientName;  
    jack_options_t options = JackNoStartServer;//(JackSessionID|JackServerName|JackNoStartServer|JackUseExactName|JackNullOption)
	jack_status_t status;

    /* open a client connection to the JACK server */
	client = jack_client_open (client_name, options, &status,server_name);
    if (status & JackNameNotUnique) {    //client name not unique, set a client name;
        client_name = jack_get_client_name(client);
        std::cerr<<"\t>>unique name "<<client_name<<" assigned to the client obj."<<std::endl;
    }
    
	if (client == NULL) {
        std::cerr<<"\t>>jack_client_open() failed, status = "<<status<<std::endl;
        if (status & JackServerFailed) {
            std::cerr<<"\t>>Unable to connect to JACK server"<<std::endl;
        }
        exit (1);
    }
    if (status & JackServerStarted) {
        std::cout<<"\t>>JACK server started"<<std::endl;
    }
}

AudioStream::~AudioStream(){
    std::cout<<"Audio stream object destructed"<<std::endl;
}
    
void AudioStream::AudioRouting(){
    
    //callback
    if (jack_set_process_callback (client,streamAudio,this)){ //arg
            std::cerr<<"\t>>Callback operation failed"<<std::endl;
    }

    //prevent failure
    jack_on_shutdown(client,&jack_shutdown,0);

    //register physical ports
    input_port = jack_port_register(client,"inputPort",JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);
    output_port_left=jack_port_register (client,"leftPort",JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    output_port_right=jack_port_register (client,"rightPort",JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    
    if ((input_port == NULL)) {
        std::cerr<<jack_port_name(input_port)<<"\t>> ERROR : no more JACK ports available"<<std::endl;
        exit (1);}
    if (output_port_left == NULL){
        std::cerr<<"\t>>Unable to register output port for {"<<jack_port_name(output_port_left)<<"}"<<std::endl;
        exit (1);}
    if (output_port_right == NULL){
        std::cerr<<"\t>>Unable to register output port for {"<<jack_port_name(output_port_right)<<"}"<<std::endl;
        exit (1);}
    
    //activate client
    if (jack_activate (client)) {
        std::cerr<<"\t>>cannot activate client {"<<client_name<<"}"<<std::endl;
        exit (1);
    }

    /*Getting acces to physical source ports*/
    fromdevice = jack_get_ports (client, NULL, NULL,JackPortIsPhysical|JackPortIsOutput);
    if (fromdevice == NULL) {
        std::cerr<<"\t>>no physical capture devices"<<std::endl;
        exit (1);
    }
// register all input device channels
    // const char **temp_device=fromdevice;
    // std::cout<<"Device list Input>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n\n\n\n"<<std::endl;
    // int devCount=0;
    // while(*temp_device!=NULL){
    //     std::cout<<devCount<<":"<<*temp_device<<" ";
    //     *temp_device++;
    //     devCount++;
    // }std::cout<<"\n\n\n\n\n"<<std::endl;
    // // (K6) output prints ...
    // //Device list Input>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // //system:capture_1 1:system:capture_2 2:system:capture_3 3:system:capture_4 4:system:capture_5 5:system:capture_6 

    /*Getting acces to destination ports*/
    todevice = jack_get_ports (client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if (todevice == NULL) {
        std::cerr<<"\t>>no physical playback devices"<<std::endl;
        exit (1);
    }

    //connect all
    if (input_port!=NULL){
        if (jack_connect (client, fromdevice[0], jack_port_name(input_port))){//returns full name
           std::cerr<<"\t>>cannot connect input device {"<<fromdevice[0]<<"} with input port {"<<jack_port_name(input_port)<<"}"<<std::endl;
    }
    }
    if (output_port_left!=NULL && output_port_right!=NULL){
        if (jack_connect (client, jack_port_name(output_port_left), todevice[0])){//returns full name
            std::cerr<<"\t>>cannot connect left plysical output port {"<<todevice[0]<<"} with input port {"<<jack_port_name(output_port_left)<<"}"<<std::endl;}
        if (jack_connect (client, jack_port_name(output_port_right), todevice[1])){//returns full name
           std::cerr<<"\t>>cannot connect right physical output port {"<<todevice[1]<<"} with input port {"<<jack_port_name(output_port_right)<<"}"<<std::endl;}
    }

    free (fromdevice);
    free (todevice);
}

void AudioStream::closeStream(){
    int simple_quit=0;
    while (!simple_quit)    /* keep running until until we get a quit event */
#ifdef WIN32
        Sleep(1*1000);
#else
        sleep(1);
#endif
    jack_client_close (client);
}

int AudioStream::streamBuffer(jack_nframes_t nframes){

    //out receive values from 
    jack_default_audio_sample_t *left,*right;

    in = (double *)jack_port_get_buffer (input_port, nframes);
    
//TODO get that signal from EACH DEVICE------> http://www.vttoth.com/CMS/index.php/technical-notes/68

    left = (jack_default_audio_sample_t *)jack_port_get_buffer (output_port_left, nframes);
    right= (jack_default_audio_sample_t *)jack_port_get_buffer(output_port_right, nframes);

    audioBufferCallback(left,right);

    // float *toneL = float[nframes];
    // float *toneR = float[nframes];
    // here apply the mixing. Note that left and right hold the sine signal returned by audioBufferCallback, so the mix should operate in place.
    // std::memcpy (left, in, sizeof (double) *nframes);
    // std::memcpy (right, in, sizeof (double) *nframes);

    return 0;
}

// void AudioStream::mix(){
//     // for a more generic app, that makes use of the input sig from a USB audio interface or the OS sys ...
//     // .. we ought to combine the two signals : the input from the channel(s) and the sine tone.
//     // In case that more than 1 input sources are connected, access the number of input sources (in particular, the #input_sources that are contributing info to the aggregated *in buffer)
// }

void AudioStream::jack_shutdown (void *arg)
{
	exit (1);
}
