#include <iostream>
#include <jack/jack.h>
#include <jack/types.h>

int main(){
    // session("thirdjam");
    // ch1=new channel()
    // io=new handshake(chname);
    const char *client_name="thirdJam";
    jack_options_t options = JackUseExactName;

    if (status & JackNameNotUnique) {    //client name not unique, set a client name;
        client_name = jack_get_client_name(client);
        std::cerr<<"unique name "<<client_name<<"assigned to the client obj."<<std::endl;
    }

    jack_client_t *client;
    client_name = jack_get_client_name(client);
    
}