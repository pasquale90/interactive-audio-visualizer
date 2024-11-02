
#include "foo.h"
#include "bar.h"
#include <gtest/gtest.h>
// #include "gtest/gtest-spi.h"

namespace {

TEST(FooTest, fooAdds) {
    Foo f(5,8);
    EXPECT_EQ(f.sum(), 13);
}

TEST(FooTest, fooSubtracts) {
    Foo f(10,3);
    EXPECT_EQ(f.subtract(), 7);
}

TEST(FooTest, fooDivides) {
    Foo f(10,3);
    EXPECT_EQ(f.div(), 3);
}

TEST(FooTest, fooMultiplies) {
    Foo f;
    EXPECT_EQ(f.mul(), 0);
}

TEST(FooTest, fooZeroDivision) {
    // EXPECT_DEATH( 
    //     Foo f(1,0);
    //     f.div();
    //     , "No 0-division allowed" );
    
    // Foo f(1,0);
    // try{
    //     f.div();
    // }
    // catch (std::logic_error e) {
    //     std::cerr << e.what();
    // }
}


TEST(FooBarTest, fooChecksBar) {
    Foo f;
    Bar b;
    EXPECT_EQ( f.checkBar(b), 0 );
}

TEST(FooBarTest, fooChecksBar2) {
    Foo f(5,15);
    Bar b(5,15);
    EXPECT_EQ( f.checkBar(b), 15 );
}

TEST(FooBarTest, fooChecksBar3) {
    int x = 3,y=4;
    Foo f(x,y);
    Bar b(y,x);
    EXPECT_EQ( f.checkBar(b), x );
}


}