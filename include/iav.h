#ifndef IAV_H
#define IAV_H

#include <thread>

#include "config.h"
#include "audioserver.h"
#include "audio.h"

class IAV{

    public:
        IAV();
        // ~IAV();
        
        void start();

    private:

        Config& cfg = Config::getInstance();
        AudioServer audioServer;
        AudioStream audioStream;

        std::thread audServerThread;
        std::thread audClientThread;
        std::thread otherTh;

        std::mutex mtxServer;
        std::condition_variable cvServer;
        bool serverStarted{false};
};


#endif