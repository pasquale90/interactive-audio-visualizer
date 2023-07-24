#include "fft.h"

// class audioVisualizer{
// };

void hamming(int windowLength, float *buffer) {
 for(int i = 0; i < windowLength; i++) {
   buffer[i] = 0.54 - (0.46 * cos( 2 * PI * (i / ((windowLength - 1) * 1.0))));
 } 
}

void computeFFT(float *signal,int bufferSize){ //float*
  fftw_plan p;
  fftw_complex in[bufferSize];
  fftw_complex out[bufferSize];
  int i;

  // Create a hamming window of appropriate length
  float window[bufferSize];
  hamming(bufferSize, window);
  
  // prepare signal
  for (i=0;i<bufferSize;++i){
    in[i][0]=signal[i]* window[i];
    in[i][1]=0;
  }

  p = fftw_plan_dft_1d(bufferSize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  fftw_execute(p);
  /* normalize */
  for (i = 0; i < bufferSize; i++) {
    in[i][0] *= 1./bufferSize;
    in[i][1] *= 1./bufferSize;
  }
  //print
  for (i = 0; i < bufferSize; i++)
    printf("recover: %3d %+9.5f %+9.5f I \n",
        i, in[i][0], in[i][1]);
  printf("\n\n\n\n\n");

  fftw_destroy_plan(p);
  fftw_cleanup();
}
