#ifndef CAMERA_HW
#define CAMERA_HW

#include <string>
#include <vector>

/*!
 * @brief Represents information about a camera.
 *
 * This structure contains the device path of the camera and a vector of pairs
 * representing supported resolutions. Each pair contains the width and height
 * of the resolution.
 * @see getAvailableCameras for more information about retrieving camera information.
*/
struct CameraInfo {
    std::string devicePath;
    std::vector<std::pair<int, int>> resolutions;
};

/*!
 * @brief Retrieves information about available cameras on the system.
 *
 * This function scans the system for connected cameras and gathers information
 * about each camera, including its device path and supported resolutions.
 *
 * @return A vector of CameraInfo structures, where each structure contains
 *         information about a single camera. If no cameras are found, an empty
 *         vector is returned.
 */
std::vector<CameraInfo> getAvailableCameras();

#endif