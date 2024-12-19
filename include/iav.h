#ifndef IAV_H
#define IAV_H

#include <thread>

#include "config.h"
#include "audioserver.h"
#include "audio.h"
#include "roi.h"
#include "videotracker.h"
#include "audiolizer.h"

class IAV{

    public:
        IAV();
        // ~IAV();
        
        void start();

    private:

        Config& cfg = Config::getInstance();
        AudioServer audioServer;
        AudioStream audioStream;
        VideoTracker videoTracker;
        Audiolizer audiolizer;

        std::thread audServerThread;
        std::thread audClientThread;
        std::thread trackingThread;
        std::thread iavThread;

        std::mutex mtxServer;
        std::condition_variable cvServer;
        bool serverStarted{false};

        // shared data
        bool trackingUpdated,patternLocked;
        RegionOfInterest trackingSig;
        cv::Mat cameraFrame;
        int frequency;

        void audiovisual();
};


#endif