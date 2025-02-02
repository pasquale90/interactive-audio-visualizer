#ifndef CAMERA_H
#define CAMERA_H

#include <atomic>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include "unused_defines.h"
struct CameraConfig;

/*!
 * @brief Class representing a camera object.
 * @note This class encapsulates the functionality for capturing frames from a camera.
 * @note The Camera class is responsible for initializing the camera and capturing frames.
 * @note The Camera class uses OpenCV library.
*/
class Camera{
public:
    
    /*! @brief Default constructor.*/
    Camera();

    /*! @brief Destructor for the camera class. It closes the camera and releases any resources.*/
    ~Camera();

    /*! @brief Method for capturing frames received from the camera.
    * @param[out] cv::Mat& - the output frame from the camera.
    */
    bool capture(cv::Mat&);

    /*! @brief Method that make use of std::atomic variable frameToggle to indicate whether a new frame elapsed
    * @return bool - true if a frame has elapsed.
    * @note This method is never used in this code.
    */
    bool frame_elapsed();

    /*!
     * @brief Copy constructor is deleted to prevent accidental use.
    */
    Camera (const Camera&) = delete;
    
    /*!
     * @brief Move constructor is deleted to prevent accidental use.
    */
    Camera (Camera&&) = delete;

    /*!
     * @brief Copy assignment operator is deleted to prevent accidental use.
    */
    Camera& operator=(const Camera&) = delete;

    /*!
     * @brief Move assignment operator is deleted to prevent accidental use.
    */    
    Camera& operator=(Camera&&) = delete;
    
private:

    CameraConfig &cameracfg;
    // int camW,camH,fps;

    std::atomic<bool> frameToggle;
    int toggleFrame;
    bool atomicChange;

    cv::VideoCapture cap;
    cv::Mat frame;

    void initialize_camera();
};





#endif