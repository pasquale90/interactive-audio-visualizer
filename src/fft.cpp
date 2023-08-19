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


double* Spectrogram::computeFFT(double *magnitude, double &minf, double &maxf){ //float*

  int i;
  double rcpVerticalZoom=1.0;
  // for (i=0;i<buffer_size;++i){
  //   std::cout<<" hamming_window[i] "<< hamming_window[i]<<std::endl; // (0.54 - (0.46 * cos( 2 * PI * (i / ((buffer_size - 1) * 1.0)))))<<std::endl; //;
  // }

  p = fftw_plan_dft_1d(height, fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);

  fftw_execute(p);

  // normalize
  // avgw/= (siglen/2);
  // std::cout<<"avg "<<avgw<<std::endl;
  minf=(20*log10f( sqrtf( fft_out[0][0]*fft_out[0][0]+ fft_out[0][1]*fft_out[i][1] ) * rcpVerticalZoom ) + 1.0f) * 0.5f;
  maxf=minf;
  
  //log magnitude
  for (i = 0; i < height; i++) {

    // fft_out[i][0] /= avgw;
    // fft_out[i][1] /= avgw;

    magnitude[i]=(20*log10f( sqrtf( fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1] ) * rcpVerticalZoom ) + 1.0f) * 0.5f;

    if (magnitude[i] < minf) minf=magnitude[i];
    if (magnitude[i] > maxf) maxf=magnitude[i];

  }

  // normalize(magnitude,min,max);
  //normalize
  // for (i = 0; i < height; i++) {
  //   magnitude[i] = (FFTcol[i]-minf)/(maxf-minf);
  // }
  // avgw=0;
}

void Spectrogram::new_approach(double *spectro){

  //requires --> fft_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2*height); --> take care of the 2*height
    int i;
    p = fftw_plan_dft_1d(2*height, fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    double rcpVerticalZoom=1.0;

    int count2   = 0;
    int max2     = 2*height + 2;
    while( count2 < max2 )
    {
        const double realValue   = fft_out[count2 + 0][0];
        const double imagValue   = fft_out[count2 + 1][1];
        const double value   = (log10f( sqrtf( (realValue * realValue) + (imagValue * imagValue) ) * rcpVerticalZoom ) + 1.0f) * 0.5f;
        spectro[count2 >> 1] = abs(value);
        count2 += 2;
    }
}

/* Last working version - artistic fft
double* Spectrogram::computeFFT(double *magnitude){ //float*

  int i;
  double rcpVerticalZoom=1.0;
  // for (i=0;i<buffer_size;++i){
  //   std::cout<<" hamming_window[i] "<< hamming_window[i]<<std::endl; // (0.54 - (0.46 * cos( 2 * PI * (i / ((buffer_size - 1) * 1.0)))))<<std::endl; //;
  // }

  p = fftw_plan_dft_1d(height, fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);

  fftw_execute(p);

  // normalize
  avgw/= (siglen/2);
  std::cout<<"avg "<<avgw<<std::endl;
  minf=(log10f( sqrtf( fft_out[0][0]*fft_out[0][0]+ fft_out[0][1]*fft_out[i][1] ) * rcpVerticalZoom ) + 1.0f) * 0.5f;
  maxf=minf;
  

  //log magnitude
  for (i = 0; i < height; i++) {
    // log_fft_out[i] = 0.5 * log( pow(fft_out[i][0],2) + pow(fft_out[i][1],2) );      //   1/2*(
    
    // fft_out[i][0] /= avgw;
    // fft_out[i][1] /= avgw;

    // FFTcol[i]=log(sqrt(fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1]));//Here I have calculated the y axis of the spectrum in dB
    FFTcol[i]=(log10f( sqrtf( fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1] ) * rcpVerticalZoom ) + 1.0f) * 0.5f;

    if (FFTcol[i] < minf) minf=FFTcol[i];
    if (FFTcol[i] > maxf) maxf=FFTcol[i];
    // fft_out[i][0]/=height;
    // fft_out[i][1]/=height;
    // magnitude[i]=abs(FFTcol[i]);
  }

  // normalize(magnitude,min,max);
  //normalize
  for (i = 0; i < height; i++) {
    magnitude[i] = (FFTcol[i]-minf)/(maxf-minf);
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
  avgw=0;
}
*/

/*
double* Spectrogram::computeFFT(double *magnitude){ //float*

  int i;
  // for (i=0;i<buffer_size;++i){
  //   std::cout<<" hamming_window[i] "<< hamming_window[i]<<std::endl; // (0.54 - (0.46 * cos( 2 * PI * (i / ((buffer_size - 1) * 1.0)))))<<std::endl; //;
  // }

  p = fftw_plan_dft_1d(height, fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);

  fftw_execute(p);

  // normalize 
  // std::cout<<"buffer_size "<<buffer_size<<" height "<<height<<std::endl;
  for (i = 0; i < height-1; i++) {
    std::cout<<"fft_out["<<i<<"][0] "<<fft_out[i][0]<<std::endl;
    std::cout<<"fft_out["<<i<<"][1] "<<fft_out[i][1]<<std::endl;
    fft_out[i][0] *= 1./height;
    fft_out[i][1] *= 1./height;
    std::cout<<"after fft_out["<<i<<"][0] "<<fft_out[i][0]<<std::endl;
    std::cout<<"after fft_out["<<i<<"][1] "<<fft_out[i][1]<<std::endl;
  }


  double minf=log(sqrt(fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1]));//sqrt(fft_out[0][0]*fft_out[0][0]+ fft_out[0][1]*fft_out[0][1]);
  double maxf=minf;
  
  //log magnitude
  for (i = 0; i < height; i++) {
    // log_fft_out[i] = 0.5 * log( pow(fft_out[i][0],2) + pow(fft_out[i][1],2) );      //   1/2*(

    FFTcol[i]=log(sqrt(fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1]));//Here I have calculated the y axis of the spectrum in dB
    // fft_out[i][0]/=height;
    // fft_out[i][1]/=height;

    // magnitude[i]=log(sqrt(fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1])/height);//Here I have calculated the y axis of the spectrum in dB
    // std::cout<<"magnitude[i] "<<magnitude[i]<<std::endl;
    // maxf = MAX (maxf, log_fft_out[i]);
    // minf = MIN (minf, log_fft_out[i]);
    if (FFTcol[i] < minf) minf=FFTcol[i];
    if (FFTcol[i] > maxf) maxf=FFTcol[i];
    // if (magnitude[i] < minf) minf=magnitude[i];
    // if (magnitude[i] > maxf) maxf=magnitude[i];
    // std::cout<<"i "<<i<<" : FFTcol[i] "<<FFTcol[i]<<std::endl;
    // std::cout<<"typeid(fft_out[i][0]).name() "<<typeid(fft_out[i][0]).name()<<std::endl;
    // std::cout<<"typeid(magnitude[i]).name() "<<typeid(magnitude[i]).name()<<std::endl;    
  }
  
  std::cout<<"minf "<<minf<<std::endl;
  std::cout<<"maxf "<<maxf<<std::endl;

  // //normalize
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

*/