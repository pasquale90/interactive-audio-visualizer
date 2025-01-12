#include "iav.h"

IAV::IAV()
{
    // Here setup everything
    audioServer.setup_server();

    audiolizer.setAudioUpdater(std::bind(&AudioStream::update,&audioStream,std::placeholders::_1,std::placeholders::_2));
    visualizer.setAudiolizerUpdater(std::bind(&Audiolizer::turn_Image_into_Sound, &audiolizer, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));

    audioStream.setupShareables(visualizer.get_waveform());

    audServerThread = std::thread (&AudioServer::start_server,&audioServer,std::ref(mtxServer), std::ref(cvServer), std::ref(serverStarted));
    audioThread = std::thread (&AudioStream::clientConnect,&audioStream,std::ref(mtxServer), std::ref(cvServer), std::ref(serverStarted));
    visualThread = std::thread(&Visualizer::broadcast,&visualizer);

    cfg.display();
}


void IAV::start(){
    // Here run all threads
    std::cout<<"Hello IAV!"<<std::endl;
    audServerThread.detach();
    audioThread.detach();
    visualThread.join();
}