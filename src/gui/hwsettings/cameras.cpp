#include <iostream>
#include <vector>
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

std::vector<CameraInfo> getAvailableCameras() {
    std::vector<CameraInfo> cameras;
    for (int i = 0; i < 16; ++i) { // Check up to 16 potential camera devices
        std::string devicePath = "/dev/video" + std::to_string(i);
        int fd = open(devicePath.c_str(), O_RDWR | O_NONBLOCK, 0);

        if (fd == -1) {
            if (errno == ENOENT || errno == EACCES) {
                continue; // Device doesn't exist or no permission, try next
            } else {
                perror("open");
                continue; // Some other error, try next
            }
        }


        struct v4l2_capability cap;
        if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
            perror("VIDIOC_QUERYCAP");
            close(fd);
            continue;
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
             //Not a video capture device
            close(fd);
            continue;
        }

        CameraInfo camera;
        camera.devicePath = devicePath;



        struct v4l2_fmtdesc fmtdesc;
        memset(&fmtdesc, 0, sizeof(fmtdesc));
        fmtdesc.index = 0;
        fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        while (ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) == 0) {
            struct v4l2_frmsizeenum frmsizeenum;
            memset(&frmsizeenum, 0, sizeof(frmsizeenum));
            frmsizeenum.index = 0;
            frmsizeenum.pixel_format = fmtdesc.pixelformat;

            while (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsizeenum) == 0) {

                if (frmsizeenum.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
                    camera.resolutions.push_back({frmsizeenum.discrete.width, frmsizeenum.discrete.height});
                } else if (frmsizeenum.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
                     for (int w = frmsizeenum.stepwise.min_width; w <= frmsizeenum.stepwise.max_width; w += frmsizeenum.stepwise.step_width) {
                        for (int h = frmsizeenum.stepwise.min_height; h <= frmsizeenum.stepwise.max_height; h += frmsizeenum.stepwise.step_height) {
                            camera.resolutions.push_back({w,h});
                        }
                    }
                }



                frmsizeenum.index++;
            }
             fmtdesc.index++;
        }

        cameras.push_back(camera);
        close(fd);


    }
    return cameras;
}





int main() {
    auto cameras = getAvailableCameras();

    for (const auto& camera : cameras) {
        std::cout << "Camera: " << camera.devicePath << std::endl;
        std::cout << "Available resolutions:" << std::endl;
        for (const auto& res : camera.resolutions) {
            std::cout << "\t" << res.first << "x" << res.second << std::endl;
        }
        std::cout << std::endl;
    }


    return 0;
}