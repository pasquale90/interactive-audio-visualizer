#ifndef CONFIG_H
#define CONFIG_H

#include "console.h"
#include "config_defaults.cpp"

struct Config{
   /*! @brief Struct constructor.
    * It receives values from the program arguments, it uses the InputParser to parse them, and passes them in a form of a struct object to the rest of the files.
    * @param int argc - number of arguments provided by the user
    * @return char* argv[] - arguments provided by the user
    */
    Config(int argc, char *argv[]);
   
   /*! @brief Default constructor.
    */
    Config();
    
    /*! @brief Copy constructor.
    */
    Config(const Config& c);

    /*! @brief Class destructor.
    */
    ~Config();
    
    /*! @brief Display function. It prints out in the console all the parameter values 
    * @return void
    */
    void display();

    int sampleRate;
    int quantization;
    int bufferSize;
    
    int minFrequency;
    int maxFrequency;

    int fps;
    int displayW;
    int displayH;
    
    int camResW;
    int camResH;
    int camfps;
    
    int radius;
    int roiSec;

    int trackingAlg;
};

#endif