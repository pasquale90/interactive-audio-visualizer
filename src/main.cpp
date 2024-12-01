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
#include "config.h"

void start_iav() {

    Config& cfg = Config::getInstance();
    cfg.display();
    std::cout<<"Hello IAV!"<<std::endl;

}

/*! @brief main function.
  * @param int argc - number of input arguments.
  * @param char **argv - arguments. No additional arguments are required. These variables are only used to initialize the QT application instance.
  * @return int - success / failure of program
  */
int main(int argc, char *argv[]){
    GUI gui(argc, argv);

    start_iav();
    return 0;
}