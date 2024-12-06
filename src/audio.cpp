#include "audio.h"

const char *server_name = nullptr;
const char clientName[17] = "IAV-audio-client";

int AudioStream::streamAudio (jack_nframes_t nframes, void *arg){ //, float *in,void (*threading)(float *sig)
    return static_cast<AudioStream*>(arg)->streamBuffer(nframes);
}

AudioStream::AudioStream(){
    client_name=clientName;
    
    // nullify all
    input_port = nullptr;
    output_port_left = nullptr;
    output_port_right = nullptr;
    client = nullptr;
    in = nullptr;
    fromdevice= nullptr;
    todevice = nullptr;
}

AudioStream::~AudioStream(){
    closeStream();
    std::cout<<"Audio stream object destructed"<<std::endl;
}

void AudioStream::clientConnect(std::mutex& mtx, std::condition_variable& cv, bool& serverStarted){

    std::cout << "Waiting for jack server to start\n";
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] { return serverStarted; });

    jack_options_t options = JackNoStartServer;//(JackSessionID|JackServerName|JackNoStartServer|JackUseExactName|JackNullOption)
	jack_status_t status;

    /* open a client connection to the JACK server */
	client = jack_client_open (client_name, options, &status,nullptr);
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

    //callback
    if (jack_set_process_callback (client,streamAudio,this)){ //arg
            std::cerr<<"\t>>Callback operation failed"<<std::endl;
    }

    //prevent failure
    jack_on_shutdown(client,&jack_shutdown,0);

    input_port = jack_port_register (client, "inputDevice",
					 JACK_DEFAULT_AUDIO_TYPE,
					 JackPortIsInput, 0);
    //register physical ports
    output_port_left=jack_port_register (client,"leftPort",JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    output_port_right=jack_port_register (client,"rightPort",JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

 //@TEMP_IMPL 
    if (input_port == NULL) {
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

 //@TEMP_IMPL
    fromdevice = jack_get_ports (client, NULL, NULL,JackPortIsPhysical|JackPortIsOutput);
    if (fromdevice == NULL) {
        std::cerr<<"\t>>no physical capture devices"<<std::endl;
        exit (1);
    }

    // Getting acces to destination ports
    todevice = jack_get_ports (client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if (todevice == NULL) {
        std::cerr<<"\t>>no physical playback devices"<<std::endl;
        exit (1);
    }

 //@TEMP_IMPL
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

    free (fromdevice); //@TEMP_IMPL
    free (todevice);
}


void AudioStream::closeStream(){
    if (jack_port_connected(input_port)){
        if(jack_port_disconnect(client,input_port)){
            std::cerr<<"Couldnt disconnect the input port from the main stream"<<std::endl;
        }
    }

    if (jack_port_connected(output_port_left)){
        if(jack_port_disconnect(client,output_port_left)){
            std::cerr<<"Couldnt disconnect the left output port from the main stream"<<std::endl;
        }
    }
    
    if (jack_port_connected(output_port_right)){
        if(jack_port_disconnect(client,output_port_right)){
            std::cerr<<"Couldnt disconnect the right output port from the main stream"<<std::endl;
        }
    }

    std::cout<<"Closing stream - turning off audio client.."<<std::endl;
    jack_client_close (client);
}

int AudioStream::streamBuffer(jack_nframes_t nframes){

    //out receive values from 
    jack_default_audio_sample_t *left,*right;

//@TEMP_IMPL
    in = static_cast<float *>(jack_port_get_buffer (input_port, nframes));

    left = static_cast<jack_default_audio_sample_t *>(jack_port_get_buffer (output_port_left, nframes));
    right= static_cast<jack_default_audio_sample_t *>(jack_port_get_buffer(output_port_right, nframes));

    // audioBufferCallback(left,right);

    // float *toneL = float[nframes];
    // float *toneR = float[nframes];
    // here apply the mixing. Note that left and right hold the sine signal returned by audioBufferCallback, so the mix should operate in place.
//@TEMP_COMMENTED
    std::memcpy (left, in, sizeof (float) *nframes);
    std::memcpy (right, in, sizeof (float) *nframes);

    return 0;
}

void AudioStream::jack_shutdown (void *arg)
{
    (void)arg; //!supress_warning unsused parameter -> Werror wont compile this.
	exit (1);
}
