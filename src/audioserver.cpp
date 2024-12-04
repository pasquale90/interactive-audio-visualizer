#include "audioserver.h"
AudioServer::AudioServer(const char* driverName):driver_name(driverName){
    server = jackctl_server_create2(NULL, NULL, NULL);
    parameters = jackctl_server_get_parameters(server);
    sigmask = jackctl_setup_signals(0);
    drivers = jackctl_server_get_drivers_list(server);

}
void AudioServer::setup_server(Config& cfg){
    
    change_server_parameters();
    print_driver_info();
    change_ALSAdriver_parameters(cfg);
}
void AudioServer::stop_server(){
    printf("\n\nShutting down server\n\n");
    jackctl_server_stop(server);
    jackctl_server_close(server);
    jackctl_server_destroy(server);
}
void AudioServer::start_server(){
    jackctl_server_open(server, jackctl_server_get_driver());
    jackctl_server_start(server);
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
void AudioServer::change_ALSAdriver_parameters(Config& cfg){
    
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
                            int sr = cfg.audconf.sampleRate.load();
                            if (jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)&sr)){
                                printf("Audioserver::change_ALSAdriver_parameters : sample rate changed succesfully to %d\n",sr);
                            }
                            // else{
                            //     printf("Audioserver::change_ALSAdriver_parameters : Reconfiguring sample rate to %d\n",sr);
                            //     // cfg.audconf.sampleRate.store(sr);
                            // }
                        }
                        // Configure device name
                        else if (!strcmp(param_name,"device")){
                            std::string device_name_str = "hw:"+cfg.audconf.audioDevice;
                            const char* device_name = device_name_str.c_str();
                            if (jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)device_name )){
                                printf("Audioserver::change_ALSAdriver_parameters : device name has changed! %s\n",device_name);
                            }
                            // else{
                            //     printf("Audioserver::change_ALSAdriver_parameters : device name has NOT changed! %s\n",device_name);
                            // }
                        }
                        // Configure buffer size
                        else if (!strcmp(param_name,"period")){
                            int buffer_size = cfg.audconf.bufferSize.load();
                            if (jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)&buffer_size)){
                                printf("Audioserver::change_ALSAdriver_parameters : buffer size has changed! %d \n",buffer_size);
                            }
                            // else{
                            //     printf("Audioserver::change_ALSAdriver_parameters : buffer size has NOT changed! %d \n",buffer_size);
                            //     cfg.audconf.bufferSize.store(buffer_size);
                            // }
                        }
                        param_ptr = jack_slist_next(param_ptr);
                    }
        }
        node_ptr = jack_slist_next(node_ptr);
    }    
}