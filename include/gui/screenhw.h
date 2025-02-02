#ifndef SCREENHW_H
#define SCREENHW_H

#include <vector>
#include <utility>
/*!
 * @brief Retrieves the resolution of the primary (default) screen using platform-specific APIs.
 * @return std::vector<std::pair<int,int>> - A list of pairs of integers representing the list of width and height supported by the screen.
 */
std::vector<std::pair<int,int>> get_screen_resolution();

#endif

