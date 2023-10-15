CC :=g++ -std=c++11 #gcc -std=c99 

OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
OBJECTS :=build/__audiolizer.o build/console.o build/config_defaults.o build/config.o build/signal.o build/camera.o build/onset.o build/btrack.o build/btracker.o build/raw.o build/visualizer.o build/fft.o build/kissfft.o build/audio.o build/main.o

JACK :=-L/usr/lib/x86_64-linux-gnu -ljack -ljackserver
FFTW :=-Ilibraries/BTrack/libs/fftw-3.3.10 -lfftw3 -lm


LOPENCV =-L/usr/lib/x86_64-linux-gnu -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_video -lopencv_imgcodecs -lopencv_features2d -lopencv_tracking
IOPENCV=-I/usr/include/opencv4

LIBSAMPLERATE :=-I/usr/local/include -lsamplerate #-L/usr/lib/x86_64-linux-gnu

BTRACK_fftw3 :=$(FFTW) -DUSE_FFTW 
BTRACK :=-Ilibraries/BTrack/src $(LIBSAMPLERATE) $(BTRACK_fftw3) 

test: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) $(JACK) $(FFT) $(LOPENCV) $(BTRACK) -lpthread -o test

build/main.o: src/main.cpp # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/main.cpp $(JACK) $(FFT) $(IOPENCV) $(BTRACK) -lpthread -o build/main.o

build/audio.o: src/audio.h src/audio.cpp
	$(CC) $(OPTIONS) -c src/audio.cpp $(JACK) -o build/audio.o

build/kissfft.o: libraries/BTrack/libs/kiss_fft130/kiss_fft.c #libraries/BTrack/libs/kiss_fft130/kiss_fft.h libraries/BTrack/libs/kiss_fft130/kissfft.hh #libraries/BTrack/libs/kiss_fft130/_kiss_fft_guts.h
	$(CC) $(OPTIONS) -c libraries/BTrack/libs/kiss_fft130/kiss_fft.c $(FFTW) -o build/kissfft.o

build/visualizer.o: src/visualizer.h src/visualizer.cpp 
	$(CC) $(OPTIONS) -c src/visualizer.cpp $(FFT) $(IOPENCV) -o build/visualizer.o $(LOPENCV)

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

build/camera.o: src/camera.h src/camera.cpp 
	$(CC) $(OPTIONS) -c src/camera.cpp $(IOPENCV) -o build/camera.o $(LOPENCV)

build/signal.o: src/signal.h src/signal.cpp
	$(CC) $(OPTIONS) -c src/signal.cpp -o build/signal.o

build/config.o: src/config.h src/config.cpp src/config_defaults.cpp
	$(CC) $(OPTIONS) -c src/config.cpp -o build/config.o

build/config_defaults.o: src/config_defaults.cpp
	$(CC) $(OPTIONS) -c src/config_defaults.cpp -o build/config_defaults.o

build/console.o: src/console.h src/console.cpp
	$(CC) $(OPTIONS) -c src/console.cpp -o build/console.o

build/__audiolizer.o: src/__audiolizer.h src/__audiolizer.cpp
	$(CC) $(OPTIONS) -c src/__audiolizer.cpp -o build/__audiolizer.o

clean:
	rm -f build/*.o test logs/*