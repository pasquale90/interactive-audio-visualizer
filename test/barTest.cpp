
#include "bar.h"
#include <gtest/gtest.h>
#include <cmath>

namespace {

TEST(BarTest, barSum) {
    Bar b(5,8);
    EXPECT_EQ(b.sum(), 5*8);
}

TEST(BarTest, barMul1) {
    Bar b;
    EXPECT_EQ(b.mul(), 0);
}

TEST(BarTest, barMul2) {
    Bar b(2,5);
    EXPECT_EQ(b.mul(), std::pow(5,2));
}

}