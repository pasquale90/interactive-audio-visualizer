#include "iav.h"

#include <iostream>
static void other(){
    char c='\0';
    while (c!='q') {
        std::cout<<"give c"<<std::endl;
        std::cin>>c;
    }
    return;
}

IAV::IAV()
{
    // Here setup everything
    audioServer.setup_server();
    cfg.display();

    audServerThread = std::thread (&AudioServer::start_server,&audioServer,std::ref(mtxServer), std::ref(cvServer), std::ref(serverStarted));
    audClientThread = std::thread (&AudioStream::clientConnect,&audioStream,std::ref(mtxServer), std::ref(cvServer), std::ref(serverStarted));
    otherTh = std::thread(&other);
    
}

// IAV::~IAV(){
//     // clean up the application     
//     // but nothing to clean up for now...
// }

void IAV::start(){
    // Here run all threads
    std::cout<<"Hello IAV!"<<std::endl;
    audServerThread.detach();
    audClientThread.detach();
    otherTh.join();
}