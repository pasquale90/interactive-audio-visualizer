#include <gtest/gtest.h>
#include "audiolizer.h"
#include "config.h"
#include "roi.h"
#include "tone.h"

class AudioLizerTest : public testing::Test {
    
    private:
        void audioUpdater(int freq, float vol){
            frequency = freq;
            volume = vol;
        }
        
    	void SetUp() override {

            audiolizer.setAudioUpdater(std::bind(&AudioLizerTest::audioUpdater, this, std::placeholders::_1, std::placeholders::_2) );
        }

	protected:
	
        Audiolizer audiolizer;

        Config& cfg = Config::getInstance();

        RegionOfInterest roi;

        int minFreq = cfg.iavconf.minFrequency;
        int maxFreq = cfg.iavconf.maxFrequency;
        int Width = cfg.camconf.camResW.load();
        int Height = cfg.camconf.camResH.load();

        Tone tone;
        int frequency {0};
        float volume {0.f};

        void setRoi(int x, int y, int w, int h){
            roi.centerX.store(x);
            roi.centerY.store(y);
            roi.volumeW.store(w);
            roi.volumeH.store(h);
        }        
};

TEST_F(AudioLizerTest, audioUpdaterSetCorrectly) {
    
    const bool pattern_locked = true;
    const bool trackingUpdated = true;
    setRoi(Width/2,Height/2,10,10);
    audiolizer.turn_Image_into_Sound(trackingUpdated, pattern_locked, roi, tone);

    EXPECT_EQ(frequency, tone.frequency.load());
    EXPECT_EQ(volume, tone.volume.load());
}

TEST_F(AudioLizerTest, trackerNotInitialized_trackingUpdatedTrue) {
    
    // pattern_locked indicates that tracker is initialized with a visual pattern. 
    const bool pattern_locked = false;
    
    const bool trackingUpdated = true;
    audiolizer.turn_Image_into_Sound(trackingUpdated, pattern_locked, roi, tone);

    EXPECT_EQ(frequency, 0);
    EXPECT_EQ(tone.frequency.load(), 0.f);
}

TEST_F(AudioLizerTest, trackerNotInitialized_trackingUpdatedFalse) {
    
    // pattern_locked indicates that tracker is initialized with a visual pattern. 
    const bool pattern_locked = false;
    
    const bool tracking_updated = false;
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);

    EXPECT_EQ(frequency, 0);
    EXPECT_EQ(tone.frequency.load(), 0.f);
}

TEST_F(AudioLizerTest, frequencyIsSet) {    
    // pattern_locked indicates that tracker is initialized with a visual pattern. 
    const bool pattern_locked = true;
    bool tracking_updated = true;
    setRoi(10,10, 10, 10);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
    EXPECT_NE(frequency, 0);
    EXPECT_NE(tone.frequency.load(), 0.f);
}

TEST_F(AudioLizerTest, trackerLostTrackingNotUpdated) {
    
    // pattern_locked indicates that tracker is initialized with a visual pattern. 
    const bool pattern_locked = true;
    bool tracking_updated = true;
    setRoi(10,10, 10, 10);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
    EXPECT_NE(frequency, 0);
    EXPECT_NE(tone.frequency.load(), 0.f);

    // store the prev frequency
    int prevFrequency = frequency;
    // set the tracker not updating its value 
    tracking_updated = false;
    // no matter what the roi currently points to, the frequency should not change
    setRoi(101,102, 103, 104);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
    EXPECT_EQ(tone.frequency.load(), prevFrequency);
    EXPECT_EQ(tone.frequency.load(), prevFrequency);

}

TEST_F(AudioLizerTest, frequencyFade) {
    
    // pattern_locked indicates that tracker is initialized with a visual pattern. 
    bool pattern_locked = true;
    const bool tracking_updated = true;
    setRoi(200,200, 200, 200);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
    EXPECT_NE(frequency, 0);
    EXPECT_NE(tone.frequency.load(), 0.f);

    // store the prev frequency
    int prevFrequency = frequency;
    // set the tracker not updating its value 
    pattern_locked = false;
    // no matter what the roi currently points to, the frequency should not change
    setRoi(101,102, 103, 104);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
    EXPECT_LT(tone.frequency.load(), prevFrequency);
    EXPECT_LT(tone.frequency.load(), prevFrequency);

}

TEST_F(AudioLizerTest, frequencyInRange) {
    const bool pattern_locked = true;
    const bool tracking_updated = true;
    for (int i = 1 ; i < Width; i++){
        setRoi(i, Height/2 , 1, 1);
        audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
        EXPECT_GT(tone.frequency.load(), minFreq);
        EXPECT_LT(tone.frequency.load(), maxFreq);
    }
}

TEST_F(AudioLizerTest, frequencyOutOfRange) {
    const bool pattern_locked = true;
    const bool tracking_updated = true;
    setRoi(Width+1, Height/2 , 1, 1);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
    EXPECT_GT(tone.frequency.load(), maxFreq);

    setRoi(-1, Height/2 , 1, 1);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
    EXPECT_LT(tone.frequency.load(), minFreq);
}
TEST_F(AudioLizerTest, volumeInRange01upTo1) {
    const bool pattern_locked = true;
    const bool tracking_updated = true;
    for (int i = 0 ; i < Height; i++){
    
        setRoi(Width/2, i, 1, 1);
        audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);
        
        EXPECT_GT(tone.volume.load(), 0.101f);
        EXPECT_LT(tone.volume.load(), 1.001f);
    }
}

TEST_F(AudioLizerTest, volumeOutOfRange) {
    const bool pattern_locked = true;
    const bool tracking_updated = true;

    setRoi(Width/2, -1, 1, 1);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);        
    EXPECT_GT(tone.volume.load(), 0.101f);

    setRoi(Width/2, Height, 1, 1);
    audiolizer.turn_Image_into_Sound(tracking_updated, pattern_locked, roi, tone);        
    EXPECT_LT(tone.volume.load(), 1.001f);
}