#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <string>
#include <atomic>

/*!
 * @brief Struct to hold audio configuration settings.
 * @note It uses atomic types for thread-safe access to these values.
 * @note This struct is used in the Config class to define audio configuration settings.
 * @see Config class for managing configuration settings.
 * @see audioDevice.- the name of the audio device.
 * @see sampleRate - the sample rate of the audio data.
 * @see quantization - the quantization of the audio data.
 * @see bufferSize - the buffer size for audio data.
 * @see numChannels - the number of output audio channels.
*/
struct AudioConfig{
    /*! @brief audioDevice - the name of the audio device.*/
    std::string audioDevice;
    /*! @brief sampleRate - the sample rate of the audio data.*/
    std::atomic<int> sampleRate;
    /* @brief quantization - the quantization of the audio data.*/
    int quantization;
    /* @brief bufferSize - the buffer size for audio data.*/
    std::atomic<int> bufferSize;
    /* @brief numChannels - the number of output audio channels.*/
    std::atomic<unsigned int> numChannels;
};

/*!
 * @brief Struct to hold camera configuration settings.
 * @note It uses atomic types for thread-safe access to these values.
 * @note This struct is used in the Config class to define camera configuration settings.
 * @see device - the name of the camera device.
 * @see frameRate - the frame rate of the camera.
 * @see camResW - the width of the camera resolution.
 * @see camResH - the height of the camera resolution.
*/
struct CameraConfig{
    /*! @brief device - the name of the camera device.*/
    std::string device;
    /* @brief frameRate - the frame rate of the camera.*/
    std::atomic<double> frameRate;
    /* @brief camResW - the width of the camera resolution.*/
    std::atomic<int> camResW;
    /* @brief camResH - the height of the camera resolution.*/
    std::atomic<int> camResH;
};

/*!
 * @brief Struct to hold display configuration settings.
 * @note It uses atomic types for thread-safe access to these values.
 * @note This struct is used in the Config class to define display configuration settings.
 * @see dispResW - the width of the display resolution.
 * @see dispResH - the height of the display resolution.
 * @see fps - the frames per second of the display.
*/
struct DisplayConfig{
    /*! @brief dispResW - the width of the display resolution.*/
    std::atomic<int> dispResW;
    /*! @brief dispResH - the height of the display resolution.*/
    std::atomic<int> dispResH;
    /* @brief fps - the frames per second of the display.*/
    std::atomic<double> fps;
};

/*!
 * @brief Struct to hold IAV configuration settings.
 * @note This struct is used in the Config class to define some custom configuration settings based on IAV application needs.
 * @see minFrequency - the minimum frequency for sound generation.
 * @see maxFrequency - the maximum frequency for sound generation.
 * @see roiRadius - the radius of the region that is used for tracking. It defines the size of it.
 * @warning trigger [currently unsupported] - the type of trigger used for tracking.
 * @see trackingAlg - the algorithm used for tracking.
 * @warning skipFramesRatio [currently unsupported] - the ratio of frames to skip during tracking.
*/
struct IAVConfig{
    /*! @brief minFrequency - the minimum frequency for sound generation.*/
    int minFrequency;
    /*! @brief maxFrequency - the maximum frequency for sound generation.*/
    int maxFrequency;  
    /* @brief roiRadius - the radius of the region that is used for tracking. It defines the size of it.*/
    int roiRadius;
    /*! @brief trigger - the type of trigger used for tracking.
     * @warning CURRENTLY UNSUPPORTED.*/
    std::string trigger;
    /*! @brief trackingAlg - the algorithm used for tracking.*/
    std::string trackingAlg;
    // int skipFramesRatio;
};


#endif