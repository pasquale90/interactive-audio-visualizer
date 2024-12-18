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
#include "iav.h" // iav.h before gui.h
#include "gui.h"

/*! @brief main function.
  * @param int argc - number of input arguments.
  * @param char **argv - arguments. No additional arguments are required. These variables are only used to initialize the QT application instance.
  * @return int - success / failure of program
  */
int main(){
    
    GUI gui;
    if (!gui.onExit()){
      IAV interactiveAudioVisualizer;
      interactiveAudioVisualizer.start();
    }
    
    return 0;
}