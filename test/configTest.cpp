#include <gtest/gtest.h>
#include "config.h"

class ConfigTest : public testing::Test {
protected:
    Config& cfg = Config::getInstance();
    bool isInitialized = (bool)cfg.audconf.sampleRate.load(); //  take one of the values to check if it is initialized or not.
                                                                // i.e. if sample rate is 0, then isInitialized is set to False.
};

TEST_F(ConfigTest,isSingleton) {
    
    Config& newInstance = Config::getInstance();
    ASSERT_EQ( &newInstance, &cfg);
}


TEST_F(ConfigTest,AudioVariablesInitialized) {

    if (isInitialized){
        EXPECT_FALSE(cfg.audconf.audioDevice.empty());
        EXPECT_NE(cfg.audconf.sampleRate.load(),0);
        EXPECT_NE(cfg.audconf.quantization, 0);
        EXPECT_NE(cfg.audconf.bufferSize.load(),0);
    } 
    else{
        EXPECT_TRUE(cfg.audconf.audioDevice.empty());
        EXPECT_EQ(cfg.audconf.sampleRate.load(),0);
        EXPECT_EQ(cfg.audconf.quantization, 0);
        EXPECT_EQ(cfg.audconf.bufferSize.load(),0);
    }
}

TEST_F(ConfigTest,CameraVariablesInitialized) {
    if (isInitialized){
        EXPECT_FALSE(cfg.camconf.device.empty());
        EXPECT_NE(cfg.camconf.frameRate.load(),0);
        EXPECT_NE(cfg.camconf.camResW.load(),0);
        EXPECT_NE(cfg.camconf.camResH.load(),0);
    }else{
        EXPECT_TRUE(cfg.camconf.device.empty());
        EXPECT_EQ(cfg.camconf.frameRate.load(),0);
        EXPECT_EQ(cfg.camconf.camResW.load(),0);
        EXPECT_EQ(cfg.camconf.camResH.load(),0);
    }
}

TEST_F(ConfigTest,DisplayVariablesInitialized) {
    if (isInitialized){
        EXPECT_NE(cfg.dispconf.dispResW.load(),0);
        EXPECT_NE(cfg.dispconf.dispResH.load(),0);
        EXPECT_NE(cfg.dispconf.fps.load(),0.);
    } else {
        EXPECT_EQ(cfg.dispconf.dispResW.load(),0);
        EXPECT_EQ(cfg.dispconf.dispResH.load(),0);
        EXPECT_EQ(cfg.dispconf.fps.load(),0.);
    }
}

TEST_F(ConfigTest,IAVariablesInitialized) {
    if (isInitialized){
        EXPECT_NE(cfg.iavconf.minFrequency,0);
        EXPECT_NE(cfg.iavconf.maxFrequency,0);
        EXPECT_NE(cfg.iavconf.roiRadius,0);
        EXPECT_FALSE(cfg.iavconf.trigger.empty());
        EXPECT_FALSE(cfg.iavconf.trackingAlg.empty());
    } else{
        EXPECT_EQ(cfg.iavconf.minFrequency,0);
        EXPECT_EQ(cfg.iavconf.maxFrequency,0);
        EXPECT_EQ(cfg.iavconf.roiRadius,0);
        EXPECT_TRUE(cfg.iavconf.trigger.empty());
        EXPECT_TRUE(cfg.iavconf.trackingAlg.empty());
    }
}
