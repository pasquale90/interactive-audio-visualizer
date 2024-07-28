
#ifndef ROI_H
#define ROI_H

struct RegionOfInterest{
    /*! the x coordinate of the center of the box */
    int centerX;
    /*! the y coordinate of the center of the box */
    int centerY;
    /*! the width of the box */
    int volumeW;
    /*! the height of the box */
    int volumeH;
};

#endif