#include <iostream>
#include <vector>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

int main() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Error: Couldn't open display." << std::endl;
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    XRRScreenResources* resources = XRRGetScreenResourcesCurrent(display, root);
    if (!resources) {
        std::cerr << "Error: Could not get screen resources." << std::endl;
        XCloseDisplay(display);
        return 1;
    }

    std::cout << "Available Screen Resolutions:" << std::endl;

    // Iterate through all modes and display valid resolutions
    for (int i = 0; i < resources->nmode; ++i) {
        XRRModeInfo* mode = &resources->modes[i];
        if (mode->width > 0 && mode->height > 0) { // Ensure valid dimensions
            std::cout << mode->width << "x" << mode->height << std::endl;
        }
    }

    XRRFreeScreenResources(resources);
    XCloseDisplay(display);

    return 0;
}