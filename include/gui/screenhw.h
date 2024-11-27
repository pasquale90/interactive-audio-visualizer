#ifndef SCREENHW_H
#define SCREENHW_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <utility>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

std::vector<std::pair<int,int>> get_screen_resolution();

#endif

