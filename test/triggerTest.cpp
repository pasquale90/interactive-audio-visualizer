#include <gtest/gtest.h>
#include "trigger.h"

class TriggerTest : public testing::Test {
    protected:
        Trigger trigger;
        bool trackingEnabled {false};
        float remaining_percentage{0.f};
        int secondsElapsed {0};
        Timer* t = trigger.getTimer();
};

// Test case for Trigger constructor
TEST_F(TriggerTest, ConstructorInitializes) {
    EXPECT_EQ(t->getRemainingTime(), photo_countdown_sec);
    trackingEnabled = trigger.isTrackingEnabled(remaining_percentage);
    EXPECT_EQ(trackingEnabled, false);
}

TEST_F(TriggerTest, ChangingMode) {
    // set the timer as finished;
    t->setTimer(-1);
    trackingEnabled = trigger.isTrackingEnabled(remaining_percentage);
    EXPECT_EQ(trackingEnabled, true);
    EXPECT_EQ(t->getRemainingTime(), experience_duration_sec);
}

TEST_F(TriggerTest, ResetFunction) {
    t->setTimer(-1);
    trackingEnabled = trigger.isTrackingEnabled(remaining_percentage);
    EXPECT_EQ(trackingEnabled, true);
    trigger.reset();
    trackingEnabled = trigger.isTrackingEnabled(remaining_percentage); 
    EXPECT_EQ(trackingEnabled, false);
}
