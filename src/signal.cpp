
#include "signal.h"

Signal::Signal(int bufferSize){
    prevfreq=0;
    sine=new double[maxFrequency];
    phaseL=0;
    phaseR=0;

}

Signal::~Signal(){
    delete[] sine;
}

// void Signal::fillBuffer(double *out1,double* out2){
// //FIIIXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
//     int FREQUENCY=200;

//     for(int i=0; i<512; i++ )
// 	{
// 		out1[i] = 0.2 * (float) sin( ((double)i/(double)FREQUENCY) * M_PI * 2. ); //data->sine[data->left_phase];  /* left */
// 		out2[i] = 0.2 * (float) sin( ((double)i/(double)FREQUENCY) * M_PI * 2. ); //data->sine[data->right_phase];  /* right */
// 		phaseL += 1;
// 		if( phaseL >= FREQUENCY ) phaseL -= FREQUENCY;
// 		phaseR += 3; /* higher pitch so we can distinguish left and right. */
// 		if( phaseR >= FREQUENCY ) phaseR -= FREQUENCY;
// 	}
// }

void Signal::prepareSine(int frequency){
//FIIIXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	
	if (frequency!=prevfreq){
		for(int i=0; i<frequency; i++ )
		{
			sine[i] = 0.2 * (double) sin( ((double)i/(double)frequency) * M_PI * 2. );
		}
		prevfreq=frequency;
	}
}

double Signal::getSineL(){
	double sampleL=sine[phaseL];
	phaseL += 1;
	if( phaseL >= prevfreq ) phaseL -= prevfreq;
	return sampleL;
}

double Signal::getSineR(){
	double sampleR=sine[phaseR];
	phaseR += 3;
	if( phaseR >= prevfreq ) phaseR -= prevfreq;
	return sampleR;
}
// int Signal::incrPhaseL(){

// }

// int Signal::incrPhaseR(){
// 	for(int i=0; i<512; i++ )
// 	{
// 		out1[i] = 0.2 * (float) sin( ((double)i/(double)frequency) * M_PI * 2. ); //data->sine[data->left_phase];  /* left */
// 		out2[i] = 0.2 * (float) sin( ((double)i/(double)frequency) * M_PI * 2. ); //data->sine[data->right_phase];  /* right */
// 		phaseL += 1;
// 		if( phaseL >= frequency ) phaseL -= frequency;
// 		phaseR += 3; /* higher pitch so we can distinguish left and right. */
// 		if( phaseR >= frequency ) phaseR -= frequency;
// 	}
// }