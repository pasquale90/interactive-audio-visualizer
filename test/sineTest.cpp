
#include <gtest/gtest.h>
#include "sine.h"
#include "tone.h"

// #include "config.h"

class SineTest : public testing::Test {
    
	private:
		void visUpdater(float value){
			visualizerBuffers[0][idx] = value;
			visualizerBuffers[1][idx] = value;
			idx++;
			idx%=bufferSize;
		}

		void SetUp() override {
			
			sine.setVisualizerUpdater(std::bind(&SineTest::visUpdater, this, std::placeholders::_1));

			audioBuffers[0] = new float[bufferSize];
			audioBuffers[1] = new float[bufferSize];
			visualizerBuffers[0] = new float[bufferSize];
			visualizerBuffers[1] = new float[bufferSize];
		}
		void TearDown() override {
			delete[] audioBuffers[0];
			delete[] audioBuffers[1];
			delete[] visualizerBuffers[0];
			delete[] visualizerBuffers[1];
		}

	protected:
	
        Sine sine;

		float *audioBuffers[2];
		float *visualizerBuffers[2];

		Config& cfg = Config::getInstance();
		int bufferSize = cfg.audconf.bufferSize.load();
		size_t idx {0};
};

TEST_F(SineTest, SetMonoSignalFunction) {
	Tone tone;
	tone.frequency.store(700);
	tone.volume.store(0.5);
    sine.setStereoSignal(tone, audioBuffers);

	for (int i=0; i<bufferSize; i++){
		EXPECT_EQ(audioBuffers[0][i], visualizerBuffers[0][i]);
	}
	
}

TEST_F(SineTest, SetStereoSignalFunction) {
	Tone tone;
	tone.frequency.store(700);
	tone.volume.store(0.5);
    sine.setStereoSignal(tone, audioBuffers);

	for (int i=0; i<bufferSize; i++){
		EXPECT_EQ(audioBuffers[0][i], visualizerBuffers[0][i]);
		EXPECT_EQ(audioBuffers[1][i], visualizerBuffers[1][i]);
	}
}


TEST_F(SineTest, SetMonoSignalFunctionZeroVolume) {
	Tone tone;
	tone.frequency.store(700);
	tone.volume.store(0.f);
    sine.setMonoSignal(tone, audioBuffers);

	for (int i=0; i<bufferSize; i++){
		EXPECT_EQ(audioBuffers[0][i], 0);
		EXPECT_EQ(visualizerBuffers[0][i],0);
	}
}

TEST_F(SineTest, SetStereoSignalFunctionZeroVolume) {
	Tone tone;
	tone.frequency.store(700);
	tone.volume.store(0.f);
    sine.setStereoSignal(tone, audioBuffers);

	for (int i=0; i<bufferSize; i++){
		EXPECT_EQ(audioBuffers[0][i], 0);
		EXPECT_EQ(visualizerBuffers[0][i],0);
		EXPECT_EQ(audioBuffers[1][i], 0);
		EXPECT_EQ(visualizerBuffers[1][i],0);
	}
}
