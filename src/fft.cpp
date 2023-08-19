#include "fft.h"

// #include <typeinfo>
// class audioVisualizer{
// };


Spetrogram::Spetrogram(){
  std::cout<<"Spetrogram object created"<<std::endl;
}

Spetrogram::Spetrogram(int bufferSize, int buffersPerFrame, int fheight) :  buffer_size(bufferSize), buffers_per_frame(buffersPerFrame) ,height(fheight) {
  
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

}

Spetrogram::~Spetrogram(){
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

void Spetrogram::hamming(int wsize){
  hamming_window= new double[wsize];
  for (int i=0;i<wsize;++i)
    hamming_window[i]=0.54-0.46*cos(2*PI*i/(wsize-1));
}
  
void Spetrogram::prepare_spectrogram(int buffCount,double *signal){

  // prepare signal
  int ctr=0;
  for (int i=buffer_size*buffCount; i < (buffer_size*(buffCount+1)) ; ++i){

    // std::cout<<" i "<<i<<" ctr "<<ctr<<std::endl;
    fft_in[i][0]=signal[ctr]*hamming_window[i]; // (0.54 - (0.46 * cos( 2 * PI * (i / ((buffer_size - 1) * 1.0)))));
    fft_in[i][1]=0;
    ctr++;   

    // std::cout<<" fft_in[i][0] "<< fft_in[i][0]<<std::endl;
    // std::cout<<" fft_in[i][1] "<< fft_in[i][1]<<std::endl;
  }
  // std::cout<<" fft_in[buffer_size*buffCount][0] == signal[0] "<< fft_in[buffer_size*buffCount][0]<<"=="<<signal[0]*hamming_window[buffer_size*buffCount]<<std::endl;
  // std::cout<<" fft_in[(buffer_size*(buffCount+1))-1][0] == signal[ctr-1] "<< fft_in[(buffer_size*(buffCount+1))-1][0]<<"=="<<signal[ctr-1]*hamming_window[(buffer_size*(buffCount+1))-1]<<std::endl;

}


double* Spetrogram::computeFFT(double *magnitude){ //float*

  int i;
  // for (i=0;i<buffer_size;++i){
  //   std::cout<<" hamming_window[i] "<< hamming_window[i]<<std::endl; // (0.54 - (0.46 * cos( 2 * PI * (i / ((buffer_size - 1) * 1.0)))))<<std::endl; //;
  // }

  p = fftw_plan_dft_1d(height, fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);

  fftw_execute(p);

  /* normalize */
  // for (i = 0; i < buffer_size; i++) {
  //   fft_out[i][0] *= 1./buffer_size;
  //   fft_out[i][1] *= 1./buffer_size;
  // }


  double minf=sqrt(fft_out[0][0]*fft_out[0][0]+ fft_out[0][1]*fft_out[0][1]);
  double maxf=minf;
  
  //log magnitude
  for (i = 0; i < height; i++) {
    // log_fft_out[i] = 0.5 * log( pow(fft_out[i][0],2) + pow(fft_out[i][1],2) );      //   1/2*(

    // log_fft_out[i]=20*log(sqrt(fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1])/height);//Here I have calculated the y axis of the spectrum in dB
    FFTcol[i]=sqrt(fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1]);//Here I have calculated the y axis of the spectrum in dB

    // maxf = MAX (maxf, log_fft_out[i]);
    // minf = MIN (minf, log_fft_out[i]);
    if (FFTcol[i] < minf) minf=FFTcol[i];
    if (FFTcol[i] > maxf) maxf=FFTcol[i];
    // std::cout<<"i "<<i<<" : FFTcol[i] "<<FFTcol[i]<<std::endl;
    // std::cout<<"typeid(fft_out[i][0]).name() "<<typeid(fft_out[i][0]).name()<<std::endl;
  }
  
  // std::cout<<"minf "<<minf<<std::endl;
  // std::cout<<"maxf "<<maxf<<std::endl;

  // double magnitude[height];
  //normalize
  for (i = 0; i < height; i++) {
    // magnitude[i] = ( 2.0 * (log_fft_out[i]-minf)/(maxf-minf)) - 1.0 ; // normalization in range [-1 1]
    magnitude[i] = (FFTcol[i]-minf)/(maxf-minf); // normalization in range [0 1]
    // std::cout<<"i "<<i<<" : magnitude[i] "<<magnitude[i]<<std::endl;
  }
  

  //print
  // for (i = 0; i < buffer_size; i++)
    // printf("fft out: %3d %+9.5f %+9.5f I \n",
        // i, fft_out[i][0], fft_out[i][1]);
        
        // if (fft_out[i][0] < -1.0 || fft_out[i][0] > 1.0)
        //   std::cout<<"i "<<i<<" : fft_out[i][0] "<<fft_out[i][0]<<std::endl;
        // if (fft_out[i][1] < -1.0 || fft_out[i][1] > 1.0)
        //   std::cout<<"i "<<i<<" : fft_out[i][1] "<<fft_out[i][1]<<std::endl;  
  // printf("\n\n\n\n\n");


  // for (i = 0; i < buffer_size; i++)
  //   printf("fft : %3d %+9.5f %+9.5f I \n",
  //       i, fft_in[i][0], fft_in[i][1]);
  // printf("\n\n\n\n\n");
    // return normed;
}
