#ifndef CAMERA_HW
#define CAMERA_HW

#include <string>
#include <vector>

struct CameraInfo {
    std::string devicePath;
    std::vector<std::pair<int, int>> resolutions;
};

std::vector<CameraInfo> getAvailableCameras();

#endif