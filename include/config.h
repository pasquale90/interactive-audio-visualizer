#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include "config_types.h"

/*!
 * @brief Singleton class to manage configuration settings, providing a unique point of access to the configuration settings.
 * @note The Config class is responsible for reading settings from the settings database and initializing objects of different configuration types.
 * @note It uses a map to store settings and provides methods to read and write them.
 * @see SettingsDB class for managing settings using a SQLite database.
*/
class Config{

public:
    
    /*! @brief Get instance of the Config class. Singleton pattern.
    * @return Config& - a reference to the Config object.
    */
    static Config& getInstance(){
        static Config config;
        return config;
    }

    /*!
     * @brief Copy constructor is deleted to fill the requirement of a singleton.
    */
    Config(Config const&)           = delete;
    
    /*!
     * @brief Copy assignment operator is deleted to fill the requirement of a singleton.
    */
    void operator=(Config const&)   = delete;

    /*! @brief Display function. It prints out in the console all the parameter values 
    * @return void
    */
    void display();

    /*!
     * @see AudioConfig struct for details on audio configuration settings.
    */
    AudioConfig audconf;

    /*!
     * @see CameraConfig struct for details on camera configuration settings.
    */
    CameraConfig camconf;
    
    /*!
     * @see DisplayConfig struct for details on display configuration settings.
    */
    DisplayConfig dispconf;
    
    /*!
     * @see IAVConfig struct for details on IAV configuration settings.
    */
    IAVConfig  iavconf;

private:
    /*! @brief Class constructor.
    * During construction the settings database will be read to initialize objects of different configuration types. 
    */
    Config();
    
    std::unordered_map<std::string, std::string> settings;

    bool runAtomicityCheck();

};

#endif