#include <iostream>
#include <unordered_set>
#include <string>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include "gui/screenhw.h"



std::vector<std::pair<int,int>> get_screen_resolution() {

    std::vector<std::pair<int,int>> screen_resolutions;
    std::unordered_set<std::string> uniquesValues;

    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Error: Couldn't open display." << std::endl;
        return {};
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    XRRScreenResources* resources = XRRGetScreenResourcesCurrent(display, root);
    if (!resources) {
        std::cerr << "Error: Could not get screen resources." << std::endl;
        XCloseDisplay(display);
        return {};
    }

    // std::cout << "Available Screen Resolutions:" << std::endl;

    // Iterate through all modes and display valid resolutions
    for (int i = 0; i < resources->nmode; ++i) {
        XRRModeInfo* mode = &resources->modes[i];
        if (mode->width > 0 && mode->height > 0) { // Ensure valid dimensions
            // std::cout << mode->width << "x" << mode->height << std::endl;
            std::string resVal = std::to_string(mode->width) +"x"+ std::to_string(mode->height);
            if(uniquesValues.find(resVal)== uniquesValues.end()){
                screen_resolutions.push_back({mode->width, mode->height});
                uniquesValues.insert(resVal);
            }
        }
    }

    XRRFreeScreenResources(resources);
    XCloseDisplay(display);

    return screen_resolutions;
}