#ifndef AUDIOSERVER_H
#define AUDIOSERVER_H
#include <mutex>
#include <condition_variable>
#include <jack/jack.h>
#include <jack/control.h>

// #define SERVER_VERBOSE

struct AudioConfig;

const char supported_driver[5] = "alsa";

/*! 
 * @brief The jack-audio server running on the alsa drivers.
 * @note This class encapsulates the functionality for managing the jack audio server and its associated resources
 * It is responsible for setting up the jack audio server, starting it, and stopping it.
 * @note The AudioServer class uses the jack library for managing the jack audio server.
 * @see the jack library documentation : https://jackaudio.org/api/
 * @see jack server example : https://github.com/jackaudio/example-clients/blob/master/server_control.c
*/
class AudioServer{
public:
    /*! @brief Default constructor
    */
    explicit AudioServer(const char* driverName = supported_driver);
    
    /*! @brief Class destructor
    * Destroys the jack audio server and its associated resources.
    */
    ~AudioServer();
    
    /*! @brief Setup the jack audio server by changing server parameters and alsa driver parameters
    * @return void
    */
    void setup_server();
    
    /*! @brief Starts the jack audio server
    * @param std::mutex& - a mutex object to control synchronization with the client.
    * @param std::condition_variable& - a condition variable object to signal when the server is ready
    * @return void
    * @warning The server has to be started before the audio client attempts to connect.
    */
    void start_server(std::mutex&, std::condition_variable&, bool&);

    /*! @brief Stops the jack audio server
    * @return void
    */
    void stop_server();

    // Rule of five (5)
    /*!
     * @brief Copy constructor is deleted to prevent accidental use.
    */
    AudioServer (const AudioServer&) = delete;

    /*!
     * @brief Move constructor is deleted to prevent accidental use.
    */
    AudioServer (AudioServer&&) = delete;

    /*!
     * @brief Copy assignment operator is deleted to prevent accidental use.
    */
    AudioServer& operator=(const AudioServer&) = delete;
    
    /*!
     * @brief Move assignment operator is deleted to prevent accidental use.
    */
    AudioServer& operator=(AudioServer&&) = delete;
    
private:
    jackctl_server_t *server;
    const JSList *parameters;
    const JSList *drivers;
    jackctl_sigmask_t *sigmask;
    const char *driver_name;
    const AudioConfig& audiocfg;

    /*! @brief Function to change the jack audio server parameters
    * @return void
    */
    void change_server_parameters();
    /*! @brief Function to change the alsa driver parameters
    * @return void
    */
    void change_ALSAdriver_parameters();
    
    /*! @brief Function to get the selected driver. In this program, the alsa driver is selected by default.
    * @return jackctl_driver_t* -  a pointer to the driver instance.
    */
    jackctl_driver_t* jackctl_server_get_driver();
    /*! @brief helper function used to get a pointer to a configurable parameter.
    * @param const JSList* - the parameters_list
    * @param const char* - the parameter's name
    * @return jackctl_parameter_t* - a pointer to a the selected parameter object
    */
    static jackctl_parameter_t* jackctl_get_parameter(const JSList*,const char *);

#ifdef SERVER_VERBOSE
    /*! @brief Function that prints server and driver parameters and can be used for configuring both server and driver on setup.
    * @param const JSList* - a pointer to a JSList that holds a table of all configurable parameters.
    * @return void
    */
    static void print_parameters(const JSList*);
    
    /*! @brief helper function to print the values of parameters.
    * @param union jackctl_parameter_value -  Type for parameter value, intentionally similar to jack_driver_param_value_t
    * @param jackctl_param_type_t - Parameter types, intentionally similar to jack_driver_param_type_t
    * @return void
    */
    static void print_value(union jackctl_parameter_value, jackctl_param_type_t);
    
    /*! @brief helper function used to print all the available information relate to the driver of interest. 
    * In this case, parameters of the ALSA driver
    * @return void
    */
    void print_driver_info();
#endif

};
#endif