
#ifndef ROI_H
#define ROI_H

#include <atomic>

struct RegionOfInterest{
    /*! the x coordinate of the center of the box */
    std::atomic<int> centerX;
    /*! the y coordinate of the center of the box */
    std::atomic<int> centerY;
    /*! the width of the box */
    std::atomic<int> volumeW;
    /*! the height of the box */
    std::atomic<int> volumeH;
};

// struct RegionOfInterest{
//     /*! the x coordinate of the center of the box */
//     std::atomic<int> centerX {0};
//     /*! the y coordinate of the center of the box */
//     std::atomic<int> centerY {0};
//     /*! the width of the box */
//     std::atomic<int> volumeW {0};
//     /*! the height of the box */
//     std::atomic<int> volumeH {0};
// };

#endif