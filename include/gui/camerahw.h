#ifndef CAMERA_HW
#define CAMERA_HW

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include <errno.h>

struct CameraInfo {
    std::string devicePath;
    std::vector<std::pair<int, int>> resolutions;
};

std::vector<CameraInfo> getAvailableCameras();

#endif