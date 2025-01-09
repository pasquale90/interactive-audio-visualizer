#include "iav.h"

IAV::IAV()
{
    // Here setup everything
    audioServer.setup_server();
    cfg.display();

    audiolizer.setAudioUpdater(std::bind(&AudioStream::update,&audioStream,std::placeholders::_1));
    
    audServerThread = std::thread (&AudioServer::start_server,&audioServer,std::ref(mtxServer), std::ref(cvServer), std::ref(serverStarted));
    audioThread = std::thread (&AudioStream::clientConnect,&audioStream,std::ref(mtxServer), std::ref(cvServer), std::ref(serverStarted));
    visualThread = std::thread(&Visualizer::broadcast,&visualizer);
    
}


void IAV::start(){
    // Here run all threads
    std::cout<<"Hello IAV!"<<std::endl;
    audServerThread.detach();
    audioThread.detach();
    visualThread.join();
}