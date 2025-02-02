#ifndef OPENCVFPS_H
#define OPENCVFPS_H

/*!
 * @brief Calculates the camera's approximate frames per second (FPS) based on OpenCV's video capture.
 *
 * @param const char* cameraDevice - The camera device id.
 * @return double - The approximate frames per second (FPS) calculated from the video capture.
 */
double getCVfps_approx(const char*);

#endif