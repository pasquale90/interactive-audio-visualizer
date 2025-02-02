
#ifndef ROI_H
#define ROI_H

#include <atomic>

/*!
 * @brief Struct to hold the region of interest (ROI) data.
 * @details Contains atomic variables for centerX, centerY, volumeW, and volumeH that allow the efficient 
 *          and concurrent access and modification of the ROI data. 
 *          This struct is used to store and manipulate the image image frame used for tracking in the videoTracker and Visualizer classes.
*/
struct RegionOfInterest{
    /*! the x coordinate of the center of the box */
    std::atomic<int> centerX {0};
    /*! the y coordinate of the center of the box */
    std::atomic<int> centerY {0};
    /*! the width of the box */
    std::atomic<int> volumeW {0};
    /*! the height of the box */
    std::atomic<int> volumeH {0};
};

#endif