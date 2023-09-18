CC :=g++ -std=c++11 #gcc -std=c99 

OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
OBJECTS :=build/onset.o build/btrack.o build/btracker.o build/raw.o build/visualizer.o build/fft.o build/kissfft.o build/audio.o build/main.o

JACK :=-L/usr/lib/x86_64-linux-gnu -ljack -ljackserver # JACK := -L/usr/lib64 -ljack -ljackserver
FFT :=-lfftw3 -lm
KISSFFT :=-Ilibraries/BTrack/libs/kiss_fft130

LOPENCV =-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_video
IOPENCV=-I/usr/local/include/opencv4

LIBSAMPLERATE :=-I/usr/local/include -lsamplerate #-L/usr/lib/x86_64-linux-gnu

BTRACK_kiss :=$(KISSFFT) -DUSE_KISS_FFT
BTRACK_fftw3 :=$(FFT) -DUSE_FFTW 
BTRACK :=-Ilibraries/BTrack/src $(LIBSAMPLERATE) $(BTRACK_fftw3) 

test: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) $(JACK) $(FFT) $(LOPENCV) $(BTRACK) -o test

build/main.o: src/main.cpp # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/main.cpp -I/usr/include $(JACK) $(FFT) $(IOPENCV) $(BTRACK) -o build/main.o

build/audio.o: src/audio.h src/audio.h #/usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/audio.cpp -I/usr/include $(JACK) -o build/audio.o

build/kissfft.o: libraries/BTrack/libs/kiss_fft130/kiss_fft.cpp #libraries/BTrack/libs/kiss_fft130/kiss_fft.h libraries/BTrack/libs/kiss_fft130/kissfft.hh #libraries/BTrack/libs/kiss_fft130/_kiss_fft_guts.h
	$(CC) $(OPTIONS) -c libraries/BTrack/libs/kiss_fft130/kiss_fft.cpp $(FFT) $(KISSFFT) -o build/kissfft.o

build/visualizer.o: src/visualizer.h src/visualizer.cpp /usr/local/include/opencv4/opencv2/opencv.hpp /usr/include/opencv4/opencv2/core/mat.hpp /usr/include/opencv4/opencv2/core.hpp
	$(CC) $(OPTIONS) -c src/visualizer.cpp -I/usr/local/include $(FFT) $(IOPENCV) -o build/visualizer.o $(LOPENCV)

build/raw.o: src/raw.h src/raw.cpp
	$(CC) $(OPTIONS) -c src/raw.cpp -o build/raw.o

build/fft.o: src/fft.h src/fft.cpp #/usr/local/include/fftw3.h # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/fft.cpp -I/usr/local/include $(FFT) -o build/fft.o  

build/btracker.o: src/beatracker.cpp src/beatracker.h
	$(CC) $(OPTIONS) -c src/beatracker.cpp $(BTRACK) -o build/btracker.o

build/btrack.o: libraries/BTrack/src/BTrack.cpp libraries/BTrack/src/BTrack.h
	$(CC) $(OPTIONS) -c libraries/BTrack/src/BTrack.cpp $(BTRACK) -o build/btrack.o

build/onset.o: libraries/BTrack/src/OnsetDetectionFunction.cpp libraries/BTrack/src/BTrack.h
	$(CC) $(OPTIONS) -c libraries/BTrack/src/OnsetDetectionFunction.cpp $(BTRACK) -o build/onset.o

clean:
	rm -f build/*.o test logs/*