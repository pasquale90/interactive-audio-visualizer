#include "audio.h"

// AudioStream myAudioStream;
void audioBufferCallback(double *in);

int streamAudio (jack_nframes_t nframes, void *arg){ //, float *in,void (*threading)(float *sig)
    return static_cast<AudioStream*>(arg)->streamBuffer(nframes);
}
AudioStream::AudioStream(const char* serverName,const char* clientName){
    
    server_name=serverName;
    client_name=clientName;  
    jack_options_t options = JackSessionID;//(JackSessionID|JackServerName|JackNoStartServer|JackUseExactName|JackNullOption)
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
//SKILL POINT get that signal from EACH DEVICE------> http://www.vttoth.com/CMS/index.php/technical-notes/68
    const char **temp_device=fromdevice;
    std::cout<<"Device list Input>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n\n\n\n"<<std::endl;
    int devCount=0;
    while(*temp_device!=NULL){
        std::cout<<devCount<<":"<<*temp_device<<" ";
        *temp_device++;
        devCount++;
    }std::cout<<"\n\n\n\n\n"<<std::endl;

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
    
    // std::cout<<"Press: \n R for rec \n S for save \n Ctl+C to quit. "<<std::endl;
    // std::string input;
    // while(std::cin>>input){
    //     if (input=="R"){
    //         std::cout<<"Storing buffer in active development"<<std::endl;
    //     }
    //     if (input=="S"){
    //         std::cout<<"Saving record in active development"<<std::endl;
    //     }
    // }
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
        
    double *left,*right;
    in = (double *)jack_port_get_buffer (input_port, nframes);

    audioBufferCallback(in);

    left = (double *)jack_port_get_buffer (output_port_left, nframes);
    right= (double *)jack_port_get_buffer(output_port_right, nframes);

    std::memcpy (left, in, sizeof (double) *nframes);
    std::memcpy (right, in, sizeof (double) *nframes);

    return 0;
}

void AudioStream::jack_shutdown (void *arg)
{
	exit (1);
}
