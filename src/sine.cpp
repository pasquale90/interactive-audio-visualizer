
#include "sine.h"
#include <cstddef>
#include <math.h>

#include "tone.h"

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#include "config.h"
Sine::Sine():audiocfg(Config::getInstance().audconf){
	rads_per_sample = 0.;
    prevfreq=0;
	phase=0.0;
}

void Sine::setVisualizerUpdater(std::function<void(float)> updater){
    updateVisualizer = std::move(updater);
}

void Sine::setMonoSignal(Tone& tone, float* monoBuffer[2]){
	
	int frequency = tone.frequency.load();
	float amplitude = tone.volume.load();
	
	if (frequency != prevfreq){	// reduce number of calculations
		rads_per_sample = (static_cast<float>(frequency * 2.* M_PI)) / static_cast<float>(audiocfg.sampleRate.load()); //radians traspotition per time unit
		prevfreq = frequency;	
	}

	for (int i=0;i<audiocfg.bufferSize.load();i++){
		float value = amplitude*(float)sin(phase);
		monoBuffer[0][i] = value;
		phase+=rads_per_sample;					// shift phase by amount of rads_per_sample
		if (phase >= 2*M_PI) phase=0;			// if phase reaches 2pi , zero it down.
		
		updateVisualizer(value); // fill the shareable ring buffer
	}	
}

void Sine::setStereoSignal(Tone& tone, float* stereoBuffer[2]){

	int frequency = tone.frequency.load();
	float amplitude = tone.volume.load();

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

		updateVisualizer(value); // fill the shareable ring buffer
	}	
}
