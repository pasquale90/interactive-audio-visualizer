#ifndef TRIGGER_H
#define TRIGGER_H

#include "config_types.h"
#include "timer.h"
struct IAVConfig;

constexpr int experience_duration_sec = 10;
constexpr int photo_countdown_sec = 5;

/*!
 * @brief A class responsible for managing the trigger behavior.
 * @note This class handles the enabling and disabling of tracking mode based on a timer.
*/
class Trigger {

    public:
        
        /*!
         * @brief Default constructor. 
         * @note Initializes a timer with a default time duration of 5 seconds.
         * @note The trigger is disabled by default.
        */
        Trigger();

        /*!
         * @brief Function used to enable or disable tracking mode.
         * 
         * The enabling and disabling of tracking mode is handled using a timer.
         * *In the future this function should be replaced with a more efficient mechanism to handle enabling and disabling based on keyboard inputs.
         *
         * @note The mode is disabled by default.
         * @param float& - True to enable tracking mode, false to disable it.
         * @return bool - True if tracking mode is enabled, false otherwise.
        */
        bool isTrackingEnabled(float&);

        /*!
         * @brief Function to reset the trigger's state to default (set tracking mode to false).
         * @return void
        */
        void reset();
        
        /*!
         * @brief Function to get a pointer to the timer object used for tracking mode.
         * @note This function is used for testing purposes.
         * @return Timer* - A pointer to the timer object.
        */
        Timer* getTimer();
    private:
        IAVConfig& iavcfg;
        Timer timer;

        int timeDurationSec;
        bool mode; // tracking mode or not
        
        void _modeToggle();
};



#endif