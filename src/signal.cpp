
#include "signal.h"

Signal::Signal(int sample_rate, int buffer_size){
	sampleRate=sample_rate;
	bufferSize=buffer_size;
    prevfreq=0;
    sine=new double[bufferSize];
    phaseL=0;
    phaseR=0;
	phase=0.0;
	amplitude = 0.1;				// not even heuristic
}

Signal::Signal(){
    prevfreq=0;
    phaseL=0;
    phaseR=0;
	phase=0.0;
	amplitude = 0.1;				// not even heuristic
}

void Signal::setup(const Config& cfg){
	sampleRate=cfg.sampleRate;
	bufferSize=cfg.bufferSize;
    sine=new double[bufferSize];
}

void Signal::erase(){
    delete[] sine;
}

void Signal::prepareSine(int frequency){
//FIIIXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE --> 0.2 is the maximum amplitude, add amplide % as arg
	/***
	 * calculation of radian step transposition per sample
	 * eq1 circles/buffer = frequency * buffer_size / SR  (i.e. in SR --> frequency amount of circles, in buffer_size --> circles?)
	 * eq2 radians_step = (circles/buffer) * 2PI / buffer_size (for each sample we divide with buffer size)
	*/
	if (frequency != prevfreq){	// reduce number of calculations
		rads_per_sample = ((double)frequency * 2.*M_PI) / (double)sampleRate; //radians traspotition per time unit
		prevfreq = frequency;	
		}

	for (int i=0;i<bufferSize;i++){

		sine[i] = amplitude*(double)sin(phase);
		phase+=rads_per_sample;					// shift phase by amount of rads_per_sample
		if (phase >= 2*M_PI) phase=0;			// if phase reaches 2pi , zero it down.
	}

}

double Signal::getSineL(){
	double sampleL=sine[phaseL];
	phaseL += 1;
	if( phaseL >= bufferSize ) phaseL = 0;
	return sampleL;
}

double Signal::getSineR(){
	double sampleR=sine[phaseR];
	phaseR += 1;	// phase shift in stereo
	if( phaseR >= bufferSize ) phaseR = 0;
	return sampleR;
}
