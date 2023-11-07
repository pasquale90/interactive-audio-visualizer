#ifndef CONSOLE_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define CONSOLE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>

class InputParser{
    public:
        InputParser (int &argc, char **argv);
        const std::string& getCmdOption(const std::string &option) const;
        bool cmdOptionExists(const std::string &option) const;

    private:
        std::vector <std::string> tokens;
};

#endif