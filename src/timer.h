#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>
using std::chrono::steady_clock;
using std::chrono::time_point;

/** @class Timer
 *  @brief Defines the low-level counter-timer used to time tasks.
 *  @details A counter timer that is also able to pause and continue for the rest of the time that is settled to last.
 */
class Timer{
public:
/** @brief Custom Constructor
  * @param int duration - the duration in seconds
  */   
    Timer(int duration);
/** @brief Class Destructor
  */ 
    ~Timer();

/** @brief inverse_count - the counter timer. The duration is initialized in the class constructor.
  * @return void
  */
    void inverse_count();
/** @brief elapsed - for when time is elapsed and the timer has finished counting.
  * @param void
  * @return True when finished.
  */
    bool elapsed();

/** @brief pause - for pausing the timer. Sets paused private field to False and breaking inverse_count method.will probably need handlers.
  * @return void
  */
    void pause();

/** @brief unpause - for continuing inverse count.
  * @return void
  */
    void unpause();

/** @brief count_time - a counter to count time elapsed.
  * @return void
  */
    void count_time(std::chrono::steady_clock::time_point then);
  
protected:
    steady_clock::time_point start_point,reach_point;
    steady_clock::time_point now;
    
private:
    //counter time duration
    int ccduration;
    int remains;
    bool paused;
    bool firstExec{true};

};

#endif // TIMER_H_INCLUDED
