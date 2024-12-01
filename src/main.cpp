/*!*******************************************//**
 * @file
 * @author Melissas Paschalis <melissaspaschalis@gmail.com>
 * @version 1.0 - demo
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
***********************************************/
#include <iostream>
#include "gui.h"



void start_iav() {

    std::cout<<"Hello IAV!"<<std::endl;
    
    SettingsDB settingsDB;
    std::unordered_map<std::string, std::string> settings = settingsDB.loadSettings();
    std::cout<<"audioDevice "<<settings["audioDevice"]<<std::endl;
    std::cout<<"sampleRate "<<settings["sampleRate"]<<std::endl;
    std::cout<<"cameraDevice "<<settings["cameraDevice"]<<std::endl;
    std::cout<<"resolution "<<settings["resolution"]<<std::endl;
    std::cout<<"bufferSize "<<settings["bufferSize"]<<std::endl;
    std::cout<<"frameRate "<<settings["frameRate"]<<std::endl;
    std::cout<<"displayResolution "<<settings["displayResolution"]<<std::endl;
    std::cout<<"displayFrameRate "<<settings["displayFrameRate"]<<std::endl;
    std::cout<<"frequencyRange "<<settings["frequencyRange"]<<std::endl;
    std::cout<<"roi "<<settings["roi"]<<std::endl;
    std::cout<<"trigger "<<settings["trigger"]<<std::endl;
    std::cout<<"trackingAlgorithm "<<settings["trackingAlgorithm"]<<std::endl;

}

/*! @brief main function.
  * Two pointers are passed to fill a sine signal using the Audiolizer, which is then passed into the Visualizer for controling image properties using audio analysis tools.
  * @param int argc - number of input arguments
  * @param char **argv - arguments. These are parsed through the InputParser class defined in console.* files.
  * @return int - success / failure of program
  */

int main(int argc, char *argv[]){
    GUI gui(argc, argv);
    start_iav();
    return 0;
}