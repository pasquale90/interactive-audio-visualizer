#include "iav.h"
#include "visualizer.h"

IAV::IAV()
{
    audioServer.setup_server();

    audiolizer.setAudioUpdater(std::bind(&AudioStream::update,&audioStream,std::placeholders::_1,std::placeholders::_2));
    visualizer.setAudiolizerUpdater(std::bind(&Audiolizer::turn_Image_into_Sound, &audiolizer, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
    audioStream.setVisualizerUpdater(std::bind(&Visualizer::updateAudioSignal, &visualizer, std::placeholders::_1));

    audServerThread = std::thread (&AudioServer::start_server,&audioServer,std::ref(mtxServer), std::ref(cvServer), std::ref(serverStarted));
    audioThread = std::thread (&AudioStream::clientConnect,&audioStream,std::ref(mtxServer), std::ref(cvServer), std::ref(serverStarted));
    visualThread = std::thread(&Visualizer::broadcast,&visualizer);

    cfg.display();
}

IAV::~IAV(){
    audioServer.~AudioServer();
    audioStream.~AudioStream();
    audiolizer.~Audiolizer() ;
    visualizer.~Visualizer();
}

void IAV::start(){
    
    audServerThread.detach();
    audioThread.detach();
    visualThread.join();

}