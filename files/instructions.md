## Instructions

Here you can find all the necessary instructions to install and run the [interactive-audio-visualizer](https://github.com/pasquale90/interactive-audio-visualizer).

### Install dependencies
To make use of interactive-audio-visualizer, the following dependencies must be installed on your system:
- [jack audio](https://jackaudio.org/) 
- [opencv](https://opencv.org/) 
- [fftw3](http://fftw.org/)
- [libsamplerate](https://github.com/libsndfile/libsamplerate)
- [BTrack](https://github.com/pasquale90/BTrack)

#### jackaudio
To install jack install both jack libjack and development tools

#### opencv
Install opencv 4.5.0 from source with contrib and GTK enabled. In a Linux system you may simply run the following script:
```
#!bin/bash
wget https://codeload.github.com/opencv/opencv/zip/refs/tags/4.5.0
wget https://codeload.github.com/opencv/opencv_contrib/zip/refs/tags/4.5.0
unzip 4.5.0
unzip 4.5.0.1
cd opencv-4.5.0
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.5.0/modules -D WITH_QT=OFF -D WITH_GTK=ON ../../opencv-4.5.0
cmake --build .
```
...and... 
```
sudo make install
```
... to install on your machine.

#### BTrack
To install BTrack, simply clone the forked repo and place it within IAV_ROOT/libraries directory, as follows:
```
cd interactive-audio-visualizer # IAV_ROOT
mkdir libraries && cd libraries
git clone https://github.com/pasquale90/BTrack
```

#### fftw3/libsamplerate
As for Linux, you may use the package manager supported by your system to install both packages and developer tools.


#### Compile
To compile the software, you must use the following steps:
```
cd interactive-audio-visualizer
mkdir build && cd build
cmake ../
cmake --build .
```
You will find the ```demo``` executable file, located within IAV_ROOTDIR.

#### Run
To run the interactive audio you must run the jack-audio server, and the configure and run the ```demo``` executable by passing various configuration options as parameters.
For convienience, all above can be executed at once by simply running:
```
bash start.sh
``` 

To quit the IAV, simply press Q.

To stop the jack audio server, simply run:
```
bash stop.sh
```


For more information you may consider mailing at melissaspaschalis@gmail.com