#include "iav.h"

#include <iostream>

void IAV::audiovisual(){
    // while(true){
        // convert tracking signal into frequency
        // bool frameElapsed=al.turn_Image_into_Sound(trackEnabled,currenTone,visualFrame,ROI, (float *)left ,  (float *)right); // rename frameElapsed with the var name "imClock"
        // visualize everything
        // exit_msg=vs.and_Sound_into_Image((float *)left, (float *)right, visualFrame, frameElapsed, trackEnabled, currenTone, ROI);

        // if exit_msg terminate app        
        // if (exit_msg){
        //     return;
        // }
    // }
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
    trackingThread = std::thread(&VideoTracker::capture,&videoTracker);
    iavThread = std::thread(&IAV::audiovisual,this);

}

// IAV::~IAV(){
//     // clean up the application     
//     // but nothing to clean up for now...
//     // videoTracker.~VideoTracker();
// }

void IAV::start(){
    // Here run all threads
    std::cout<<"Hello IAV!"<<std::endl;
    audServerThread.detach();
    audClientThread.detach();
    trackingThread.detach();
    iavThread.join();
}