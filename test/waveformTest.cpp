#include <gtest/gtest.h>
#include "waveform.h"

TEST(WaveFIFOtest, isEmptyFunction) {
    Waveform waveform;
    
    EXPECT_TRUE(waveform.isEmpty());
    
    waveform.write(static_cast<float>(0));
    EXPECT_FALSE(waveform.isEmpty());
    
    float val;
    waveform.read(val);
    EXPECT_TRUE(waveform.isEmpty());
}

TEST(WaveFIFOtest, isFullFunction) {
    Waveform waveform;
    for(size_t i = 0; i < waveform.size()-2 ; i++){
        waveform.write(static_cast<float>(i));
        EXPECT_FALSE(waveform.isFull());
    }
    waveform.write(static_cast<float>(0));
    EXPECT_TRUE(waveform.isFull());
}

TEST(WaveFIFOtest, availableForReadingFunction) {
    Waveform waveform;
    
    int samplesForWriting = waveform.size()/2;

    for (int i = 0; i < samplesForWriting; i++){
        waveform.write(static_cast<float>(i));
    }
    
    EXPECT_EQ(waveform.availableForReading(), samplesForWriting);
}

TEST(WaveFIFOtest, readWriteFunction) {
    Waveform waveform;
    
    int N{30};
    
    for (int i = 0; i < N; i++){
        waveform.write(static_cast<float>(i));
    }
    
    float val;
    for (int i = 0; i < N; i++){
        EXPECT_TRUE(waveform.read(val));
        EXPECT_EQ(val, static_cast<float>(i));
    }

}

TEST(WaveFIFOtest, getMinMaxFunction) {
    Waveform waveform;
    
    int N{30};
    
    for (int i = 0; i < N; i++){
        waveform.write(static_cast<float>(i));
    }
    
    float minMax[2];
    waveform.getMinMax(minMax);
    
    EXPECT_EQ(minMax[0], 0.0);
    EXPECT_EQ(minMax[1], static_cast<float>(N-1));
}