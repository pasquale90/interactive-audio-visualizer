CC :=g++ -std=c++11 #gcc -std=c99 

OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
OBJECTS :=build/onset.o build/btrack.o build/btracker.o build/visualizer.o build/fft.o build/kissfft.o build/audio.o build/main.o

JACK :=-L/usr/lib/x86_64-linux-gnu -ljack -ljackserver # JACK := -L/usr/lib64 -ljack -ljackserver
FFT :=-lfftw3
KISSFFT :=-Ilibs/BTrack/libs/kiss_fft130

LOPENCV =-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_video
IOPENCV=-I/usr/local/include/opencv4

LIBSAMPLERATE :=-I/usr/local/include -lsamplerate #-L/usr/lib/x86_64-linux-gnu
# KISSFFT :=-I/home/sonic_dev/Desktop/btack/BTrack/libs/kiss_fft130
BTRACK :=-Ilibs/BTrack/src  #-DUSE_FFTW 

test: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) $(JACK) $(FFT) $(KISSFFT) $(LOPENCV) $(LIBSAMPLERATE) $(BTRACK) -o test -DUSE_KISS_FFT

build/main.o: src/main.cpp # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/main.cpp -I/usr/include $(JACK) $(FFT) $(KISSFFT) $(IOPENCV) $(LIBSAMPLERATE) $(BTRACK) -o build/main.o -DUSE_KISS_FFT

build/audio.o: src/audio.h src/audio.h #/usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/audio.cpp -I/usr/include $(JACK) -o build/audio.o

build/kissfft.o: libs/BTrack/libs/kiss_fft130/kiss_fft.cpp #libs/BTrack/libs/kiss_fft130/kiss_fft.h libs/BTrack/libs/kiss_fft130/kissfft.hh #libs/BTrack/libs/kiss_fft130/_kiss_fft_guts.h
	$(CC) $(OPTIONS) -c libs/BTrack/libs/kiss_fft130/kiss_fft.cpp $(FFT) $(KISSFFT) -o build/kissfft.o

build/fft.o: src/fft.h src/fft.cpp #/usr/local/include/fftw3.h # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/fft.cpp -I/usr/local/include $(FFT) -o build/fft.o

build/visualizer.o: src/visualizer.h src/visualizer.cpp /usr/local/include/opencv4/opencv2/opencv.hpp /usr/include/opencv4/opencv2/core/mat.hpp /usr/include/opencv4/opencv2/core.hpp
	$(CC) $(OPTIONS) -c src/visualizer.cpp $(IOPENCV) -o build/visualizer.o $(LOPENCV)

build/btracker.o: src/beatracker.cpp src/beatracker.h
	$(CC) $(OPTIONS) -c src/beatracker.cpp $(LIBSAMPLERATE) $(KISSFFT) $(BTRACK) -o build/btracker.o -DUSE_KISS_FFT

build/btrack.o: libs/BTrack/src/BTrack.cpp libs/BTrack/src/BTrack.h
	$(CC) $(OPTIONS) -c libs/BTrack/src/BTrack.cpp $(LIBSAMPLERATE) $(KISSFFT)  $(BTRACK) -o build/btrack.o -DUSE_KISS_FFT

build/onset.o: libs/BTrack/src/OnsetDetectionFunction.cpp libs/BTrack/src/BTrack.h
	$(CC) $(OPTIONS) -c libs/BTrack/src/OnsetDetectionFunction.cpp $(LIBSAMPLERATE) $(KISSFFT)  $(BTRACK) -o build/onset.o -DUSE_KISS_FFT

clean:
	rm -f build/*.o test logs/*