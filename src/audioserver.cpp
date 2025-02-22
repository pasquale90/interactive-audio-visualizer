#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include "audioserver.h"
#include "config.h"

AudioServer::AudioServer(const char* driverName):driver_name(driverName),audiocfg (Config::getInstance().audconf){
    server = jackctl_server_create2(NULL, NULL, NULL);
    parameters = jackctl_server_get_parameters(server);
    sigmask = jackctl_setup_signals(0);
    drivers = jackctl_server_get_drivers_list(server);

}

AudioServer::~AudioServer(){
    printf("Stopping server\n");
    stop_server();
}

void AudioServer::setup_server(){
    
    change_server_parameters();
#ifdef SERVER_VERBOSE
    print_driver_info();
#endif
    change_ALSAdriver_parameters();
}
void AudioServer::stop_server(){
    printf("\n\nShutting down server\n\n");
    jackctl_server_stop(server);
    jackctl_server_close(server);
    jackctl_server_destroy(server);
}
void AudioServer::start_server(std::mutex& mtx, std::condition_variable& cv, bool& serverStarted){
    jackctl_server_open(server, jackctl_server_get_driver());
    jackctl_server_start(server);

    // Signal that server has been started
    {
        std::lock_guard<std::mutex> lock(mtx);
        serverStarted = true;
    }
    cv.notify_one();

    jackctl_wait_signals(sigmask);
}
void AudioServer::change_server_parameters(){
    // change server param --> make verbose
    jackctl_parameter_t* param;
    union jackctl_parameter_value value;
    param = jackctl_get_parameter(parameters, "verbose");
    if (param != NULL) {
        value.b = false;//true;
        jackctl_parameter_set_value(param, &value);
    }
    // change server param --> make real-time
    param = jackctl_get_parameter(parameters, "realtime");
    if (param != NULL) {
        value.b = true;
        jackctl_parameter_set_value(param, &value);
        printf("Success on changing real time");
    }
    // change server param --> change real-time priority
    param = jackctl_get_parameter(parameters, "realtime-priority");
    if (param != NULL) {
        value.b = 80;
        jackctl_parameter_set_value(param, &value);
        printf("Success on changing real-time priority");
    }
}

jackctl_driver_t* AudioServer::jackctl_server_get_driver()
{
    const JSList * node_ptr = drivers;
    while (node_ptr) {
        if (strcmp(jackctl_driver_get_name(static_cast<jackctl_driver_t *>(node_ptr->data)), driver_name) == 0) {
            return (jackctl_driver_t *)node_ptr->data;
        }
        node_ptr = jack_slist_next(node_ptr);
    }
    return NULL;
}
jackctl_parameter_t* AudioServer::jackctl_get_parameter(const JSList * parameters_list,const char * parameter_name){
    while (parameters_list)
    {
        if (strcmp(jackctl_parameter_get_name(static_cast<jackctl_parameter_t *>(parameters_list->data)), parameter_name) == 0)
        {
            return (jackctl_parameter_t *)parameters_list->data;
        }
        parameters_list = jack_slist_next(parameters_list);
    }
    return NULL;
}

#ifdef SERVER_VERBOSE

void AudioServer::print_parameters(const JSList * node_ptr)
{
    while (node_ptr != NULL) {
        jackctl_parameter_t * parameter = static_cast<jackctl_parameter_t*>(node_ptr->data);
        printf("\nparameter name = %s\n", jackctl_parameter_get_name(parameter));
        printf("parameter id = %c\n", jackctl_parameter_get_id(parameter));
        printf("parameter short decs = %s\n", jackctl_parameter_get_short_description(parameter));
        printf("parameter long decs = %s\n", jackctl_parameter_get_long_description(parameter));
        print_value(jackctl_parameter_get_default_value(parameter), jackctl_parameter_get_type(parameter));
        node_ptr = jack_slist_next(node_ptr);
    }
}

void AudioServer::print_value(union jackctl_parameter_value value, jackctl_param_type_t type){
    switch (type) {
    
        case JackParamInt:
            printf("parameter value = %d\n", value.i);
            break;
            
         case JackParamUInt:
            printf("parameter value = %u\n", value.ui);
            break;
            
         case JackParamChar:
            printf("parameter value = %c\n", value.c);
            break;
        
         case JackParamString:
            printf("parameter value = %s\n", value.str);
            break;
            
         case JackParamBool:
            printf("parameter value = %d\n", value.b);
            break;
     }
}

void AudioServer::print_driver_info(){
    const JSList * node_ptr = drivers;
    while (node_ptr != NULL) {
        jackctl_driver_t *driver = static_cast<jackctl_driver_t *>(node_ptr->data);
        if (!strcmp(jackctl_driver_get_name(driver),driver_name)){
            printf("\n--------------------------\n");
            printf("driver = %s\n", jackctl_driver_get_name(driver));
            printf("-------------------------- \n");
            print_parameters(jackctl_driver_get_parameters(driver)); 
        }
        node_ptr = jack_slist_next(node_ptr);
    }    
}
#endif

void AudioServer::change_ALSAdriver_parameters(){
    const JSList * node_ptr = drivers;
    while (node_ptr != NULL) {
        
        jackctl_driver_t *driver = static_cast<jackctl_driver_t *>(node_ptr->data);
        if (!strcmp(jackctl_driver_get_name(driver),driver_name)){
                    const JSList * param_ptr = jackctl_driver_get_parameters(driver);
                    while (param_ptr != NULL) {
                        
                        jackctl_parameter_t * parameter = static_cast<jackctl_parameter_t *>(param_ptr->data);
                        const char* param_name = jackctl_parameter_get_name(parameter);
                        // Configure sample rate
                        if (!strcmp(param_name,"rate")){
                            int sr = audiocfg.sampleRate.load();
                            if (jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)&sr)){
                                printf("Audioserver::change_ALSAdriver_parameters : sample rate changed succesfully to %d\n",sr);
                            }
                            // else{
                            //     jackctl_parameter_value jpv = jackctl_parameter_get_value(parameter);
                            //     cfg.audconf.sampleRate.store(static_cast<int>(jpv.ui));
                            //     printf("Audioserver::change_ALSAdriver_parameters : Reconfiguring sample rate to %d\n",cfg.audconf.sampleRate.load());
                            // }
                        }
                        // Configure device name
                        else if (!strcmp(param_name,"device")){
                            std::string device_name_str = "hw:"+audiocfg.audioDevice;
                            const char* device_name = device_name_str.c_str();
                            if (jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)device_name )){
                                printf("Audioserver::change_ALSAdriver_parameters : device name has changed to: %s\n",device_name);
                            }
                            // else{
                            //     jackctl_parameter_value defaultDevice = jackctl_parameter_get_default_value(parameter);
                            //     cfg.audconf.audioDevice = defaultDevice.str;
                            //     printf("Audioserver::change_ALSAdriver_parameters : configuring default device to :  %s\n",cfg.audconf.audioDevice.c_str());
                            // }
                        }
                        // Configure buffer size
                        else if (!strcmp(param_name,"period")){
                            int buffer_size = audiocfg.bufferSize.load();
                            if (jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)&buffer_size)){
                                printf("Audioserver::change_ALSAdriver_parameters : buffer size has changed to: %d \n",buffer_size);
                            }
                            // else{
                            //     jackctl_parameter_value jpv = jackctl_parameter_get_value(parameter);
                            //     cfg.audconf.bufferSize.store(static_cast<int>(jpv.ui));
                            //     printf("Audioserver::change_ALSAdriver_parameters : buffer size has NOT changed. Current buffer size value : %d \n",cfg.audconf.bufferSize.load());
                            // }
                        }
                        param_ptr = jack_slist_next(param_ptr);
                    }
        }
        node_ptr = jack_slist_next(node_ptr);
    }    
}