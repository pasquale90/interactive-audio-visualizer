
#include "sine.h"
#include <cstddef>
#include <math.h>

#include "waveform.h"

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#include "config.h"
Sine::Sine():audiocfg(Config::getInstance().audconf){
	rads_per_sample = 0.;
    prevfreq=0;
	phase=0.0;
// @FIX
	amplitude = 0.1;	
}

void Sine::setupShareables(const std::shared_ptr<Waveform>& fifo){
    waveform = fifo;
}

void Sine::setMonoSignal(int frequency, float* monoBuffer[2]){

	if (frequency != prevfreq){	// reduce number of calculations
		rads_per_sample = (static_cast<float>(frequency * 2.* M_PI)) / static_cast<float>(audiocfg.sampleRate.load()); //radians traspotition per time unit
		prevfreq = frequency;	
	}

	for (int i=0;i<audiocfg.bufferSize.load();i++){
		float value = amplitude*(float)sin(phase);
		monoBuffer[0][i] = value;
		phase+=rads_per_sample;					// shift phase by amount of rads_per_sample
		if (phase >= 2*M_PI) phase=0;			// if phase reaches 2pi , zero it down.

		waveform->write(value); // fill the shareable ring buffer
	}	
	waveform->bufferCount++;
}

void Sine::setStereoSignal(int frequency, float* stereoBuffer[2]){

	if (frequency != prevfreq){	// reduce number of calculations
		rads_per_sample = (static_cast<float>(frequency * 2. * M_PI)) / static_cast<float>(audiocfg.sampleRate.load()); //radians traspotition per time unit
		prevfreq = frequency;	
	}

	for (int i=0;i<audiocfg.bufferSize.load();i++){
		float value = amplitude*(float)sin(phase);
		stereoBuffer[0][i] = value;
		stereoBuffer[1][i] = value;
		phase+=rads_per_sample;					// shift phase by amount of rads_per_sample
		if (phase >= 2*M_PI) phase=0;			// if phase reaches 2pi , zero it down.

		waveform->write(value); // fill the shareable ring buffer
	}	
	waveform->bufferCount++;

}
