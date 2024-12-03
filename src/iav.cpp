#include "iav.h"

#include <iostream>

IAV::IAV()
{
    // Here setup everything
    audioServer.setup_server(cfg);
    cfg.display();
}

IAV::~IAV(){
    // clean up the application
    audioServer.stop_server();
}

void IAV::start(){
    // Here run all threads
    std::cout<<"Hello IAV!"<<std::endl;
    audioServer.start_server();
}