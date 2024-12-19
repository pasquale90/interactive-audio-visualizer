#include "iav.h"

// #include <iostream>
#include <cstdio>
#include <ncurses.h>

void IAV::audiovisual(){

    // @TEMP ncurses for getting the exit sig
    initscr();
    nodelay(stdscr, TRUE);  // Set non-blocking
    cbreak();
    noecho();
    char c = '\0';

    while (c != 'q') {
        int key = getch();
        c = static_cast<char>(key);
        clear(); 
        mvprintw(0, 0, "Press 'q' to quit.");
        refresh();  

        // update videoTracker's shared data  
        trackingUpdated = videoTracker.update(trackingSig,cameraFrame,patternLocked);
  
        // convert tracking signal into frequency
        bool frequencyChanged = audiolizer.turn_Image_into_Sound(trackingUpdated, patternLocked, trackingSig, frequency);
        
        // update audioStream with the newFrequency
        if (frequencyChanged){
            audioStream.update(frequency);
        }
        


        // visualize everything
        // exit_msg=vs.and_Sound_into_Image((float *)left, (float *)right, visualFrame, frameElapsed, trackEnabled, currenTone, ROI);

        // if exit_msg terminate app        
        // if (exit_msg){
        //     return;
        // }

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    endwin();
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

    // initialize shared data
    frequency=0;
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