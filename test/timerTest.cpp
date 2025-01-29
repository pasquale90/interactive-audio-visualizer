#include <gtest/gtest.h>
#include "timer.h"


TEST(TimerTest, ConstructorInitializes) {
    Timer timer;
    EXPECT_EQ(timer.getRemainingTime(), 0);
    EXPECT_EQ(timer.isTimerFinished(), true);
}

TEST(TimerTest, SetTimerFunction_ValidInput) {
    Timer timer;
    timer.setTimer(5);
    EXPECT_EQ(timer.getRemainingTime(), 5);
    EXPECT_EQ(timer.isTimerFinished(), false);
}

TEST(TimerTest, SetTimerFunction_NegativeInput) {
    Timer timer;
    timer.setTimer(-1);
    EXPECT_EQ(timer.getRemainingTime(), -1);
    EXPECT_EQ(timer.isTimerFinished(), true);
}

TEST(TimerTest, GetCurrentTimeFunction) {
    Timer timer;
    timer.setTimer(5);
    timer.start();
    int secondsElapsed;
    EXPECT_EQ(timer.getCurrentTime(secondsElapsed), false);
}

TEST(TimerTest, GetCurrentTimeFunction_Finished) {
    Timer timer;
    timer.setTimer(-1);
    timer.start();
    int secondsElapsed;
    EXPECT_EQ(timer.getCurrentTime(secondsElapsed), true);
}

