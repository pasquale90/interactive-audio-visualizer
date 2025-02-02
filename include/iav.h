#ifndef IAV_H
#define IAV_H

#include <thread>
#include "config.h"
#include "audioserver.h"
#include "audio.h"
#include "audiolizer.h"
#include "visualizer.h"

/*!
 * @brief Class to manage the IAV multi-threaded processing pipeline.
 * @see AudioServer class for the audio server operations.
 * @see AudioStream class for audio processing and streaming.
 * @see Visualizer class for visualizing audio data.
 * @see Audiolizer class for converting visual stimulus into audio related data.
 * @see Config class for managing configuration settings.
*/
class IAV{

    public:

        /*!
         * @brief Default constructor.
        */
        IAV();

        /*!
         * @brief Class destructor.
        */
        ~IAV();
        
        /*!
         * @brief Starts the IAV processing pipeline.
         * @return void
        */
        void start();
        
        /*!
        * @brief Copy constructor is deleted to prevent accidental use.
        */
        IAV(const IAV&) = delete;
        
        /*!
         * @brief Move constructor is deleted to prevent accidental use.
         */
        IAV(IAV&&) = delete;
        
        /*!
         * @brief Copy assignment operator is deleted to prevent accidental use.
         */
        IAV& operator=(const IAV&) = delete;
        
        /*!
         * @brief Move assignment operator is deleted to prevent accidental use.
         */
        IAV&& operator=(IAV&&) = delete;
        
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