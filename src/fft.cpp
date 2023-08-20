#include "fft.h"

// #include <typeinfo>
// class audioVisualizer{
// };


Spectrogram::Spectrogram(){
  std::cout<<"Spetrogram object created"<<std::endl;
}

Spectrogram::Spectrogram(int bufferSize, int buffersPerFrame, int fheight) :  buffer_size(bufferSize), buffers_per_frame(buffersPerFrame) ,height(fheight) {
  
  // fft_in=new fftw_complex[buffer_size];
  // fft_out=new fftw_complex[buffer_size];
  // fftw_complex fft_in[buffer_size];
  // fftw_complex fft_out[buffer_size];

  // Create a hamming window of appropriate length
  
  
  FFTcol = new double[height];
  siglen = buffer_size*buffers_per_frame;
  
  hamming(siglen);

  fft_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * siglen);
  fft_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height);
  std::cout<<"Spetrogram initialized with buffer size "<<buffer_size<<" and height "<<height<<std::endl;

  avgw=0;
}

Spectrogram::~Spectrogram(){
  // fftw_destroy_plan(p);
  fftw_destroy_plan(p);
  fftw_cleanup();
  delete[] hamming_window;
  // delete[] fft_in;
  // delete[] fft_out;
  fftw_free(fft_in); fftw_free(fft_out);
  delete[] FFTcol;

  std::cout<<"Spetrogram object destroyed"<<std::endl;
}

void Spectrogram::hamming(int wsize){
  hamming_window= new double[wsize];
  for (int i=0;i<wsize;++i)
    hamming_window[i]=0.54-0.46*cos(2*PI*i/(wsize-1));
}

// fft is calculated once per frame and each audio buffer occured up till then is appended to the fft input signal to increase the amount of data.
void Spectrogram::prepare_spectrogram(int buffCount,double *signal){

  // prepare signal
  int ctr=0;
  for (int i=buffer_size*buffCount; i < (buffer_size*(buffCount+1)) ; ++i){

    // std::cout<<" i "<<i<<" ctr "<<ctr<<std::endl;
    fft_in[i][0]=signal[ctr]*hamming_window[i]; // (0.54 - (0.46 * cos( 2 * PI * (i / ((buffer_size - 1) * 1.0)))));
    fft_in[i][1]=0;
    avgw+=fft_in[i][0];
    
    ctr++;   

    // std::cout<<" fft_in[i][0] "<< fft_in[i][0]<<std::endl;
    // std::cout<<" fft_in[i][1] "<< fft_in[i][1]<<std::endl;
  }
  // std::cout<<" fft_in[buffer_size*buffCount][0] == signal[0] "<< fft_in[buffer_size*buffCount][0]<<"=="<<signal[0]*hamming_window[buffer_size*buffCount]<<std::endl;
  // std::cout<<" fft_in[(buffer_size*(buffCount+1))-1][0] == signal[ctr-1] "<< fft_in[(buffer_size*(buffCount+1))-1][0]<<"=="<<signal[ctr-1]*hamming_window[(buffer_size*(buffCount+1))-1]<<std::endl;

}


void Spectrogram::computeFFT(double *magnitude, double &minf, double &maxf){ //float*

  int i;
  double rcpVerticalZoom=1.0;

  p = fftw_plan_dft_1d(height, fft_in, fft_out, FFTW_FORWARD, FFTW_MEASURE); //FFTW_ESTIMATE

  fftw_execute(p);

  minf=(20*log10f( sqrtf( fft_out[0][0]*fft_out[0][0]+ fft_out[0][1]*fft_out[i][1] ) * rcpVerticalZoom ) + 1.0f) * 0.5f;
  maxf=minf;
  
  //log magnitude
  for (i = 0; i < height; i++) {

    magnitude[i]=(20*log10f( sqrtf( fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1] ) * rcpVerticalZoom ) + 1.0f) * 0.5f;

    if (magnitude[i] < minf) minf=magnitude[i];
    if (magnitude[i] > maxf) maxf=magnitude[i];

  }
}