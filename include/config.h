#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include "config_types.h"

class Config{

public:
    
    static Config& getInstance(){
        static Config config;
        return config;
    }

    Config(Config const&)           = delete;
    void operator=(Config const&)   = delete;

    /*! @brief Display function. It prints out in the console all the parameter values 
    * @return void
    */
    void display();

    AudioConfig audconf;
    CameraConfig camconf;
    DisplayConfig dispconf;
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