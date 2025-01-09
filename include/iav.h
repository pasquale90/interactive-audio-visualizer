#ifndef IAV_H
#define IAV_H

#include <thread>

#include "config.h"
#include "audioserver.h"
#include "audio.h"
#include "audiolizer.h"
#include "visualizer.h"

class IAV{

    public:
        IAV();
        void start();

    private:

        Config& cfg = Config::getInstance();
        AudioServer audioServer;
        AudioStream audioStream;
        Audiolizer audiolizer;
        Visualizer visualizer;
        
        std::thread audServerThread;
        std::thread audioThread;
        std::thread visualThread;

        std::mutex mtxServer;
        std::condition_variable cvServer;
        bool serverStarted{false};

};


#endif