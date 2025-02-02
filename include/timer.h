#ifndef TIMER_H
#define TIMER_H

#include <chrono>

/*!
 * @brief A class responsible for managing a timer.
*/
class Timer {

    public:

        /*!
         * @brief Default constructor. Initializes the timer with a default time of 0 seconds.
        */
        Timer();

        /*!
         * @brief Sets the timer to a specific time in seconds.
         * @param seconds - The new time in seconds for the timer.
         * @return void
        */
        void setTimer(int);

        /*!
         * @brief Starts the timer.
         * @return void
        */
        void start();

        /*!
         * @brief Updates the timer.
         * @return void
        */
        bool update(int&);

        /*!
         * @brief Gets the remaining time for display or logging purposes.
         * @return int - The remaining time in seconds.
         */
        int getRemainingTime() const;

        /*!
         * @brief Checks if the timer has finished.
         * @return bool - True if the timer has finished, false otherwise.
         */
        bool isTimerFinished() const;

    private:

        int initialSeconds;                     // The initial time to start the countdown from (in seconds)
        int remainingTimeMilliseconds;          // Remaining time in milliseconds
        std::chrono::steady_clock::time_point lastUpdateTime;  // Last frame timestamp to calculate elapsed time

};



#endif