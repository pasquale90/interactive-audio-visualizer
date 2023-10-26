
#include "signal.h"

Signal::Signal(int bufferSize, int maxFrequency){
    prevfreq=0;
    sine=new double[512];
    phaseL=0;
    phaseR=0;
	phase=0.0;

}

Signal::~Signal(){
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
		rads_per_sample = ((double)frequency * 2.*M_PI) / (double)44100; //radians traspotition per time unit
		prevfreq = frequency;	
		}

	for (int i=0;i<512;i++){

		sine[i] = 1.0*(double)sin(phase);
		phase+=rads_per_sample;					// shift phase by amount of rads_per_sample
		if (phase >= 2*M_PI) phase=0;			// if phase reaches 2pi , zero it down.
	}

}

double Signal::getSineL(){
	double sampleL=sine[phaseL];
	phaseL += 1;
	if( phaseL >= 512 ) phaseL = 0;
	return sampleL;
}

double Signal::getSineR(){
	double sampleR=sine[phaseR];
	phaseR += 3;	// phase shift in stereo
	if( phaseR >= 512 ) phaseR = 0;
	return sampleR;
}
