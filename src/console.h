#ifndef CONSOLE_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define CONSOLE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>

class InputParser{
    public:
        /*! @brief Class constructor
         * @param int &argc - number of arguments
         * @param char **argv - arguments provided by the user
         */
        InputParser (int &argc, char **argv);

        /*! @brief Method to return the value of a certain key */
        const std::string& getCmdOption(const std::string &option) const;

        /*! @brief Method to control valid arguments */
        bool cmdOptionExists(const std::string &option) const;

    private:
        std::vector <std::string> tokens;
};

#endif