CC :=g++ -std=c++11 #gcc -std=c99 
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
OBJECTS :=build/visualizer.o build/fft.o build/audio.o build/main.o

JACK :=-L/usr/lib/x86_64-linux-gnu/ -ljack -ljackserver # JACK := -L/usr/lib64 -ljack -ljackserver
FFT :=-lfftw3

LOPENCV =-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_video
IOPENCV=-I/usr/local/include/opencv4

test: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) $(JACK) $(FFT) $(LOPENCV) -o test 

build/main.o: src/main.cpp # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/main.cpp -I/usr/include $(JACK) $(FFT) $(IOPENCV) -o build/main.o 

build/audio.o: src/audio.h src/audio.h #/usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/audio.cpp -I/usr/include $(JACK) -o build/audio.o

build/fft.o: src/fft.h src/fft.cpp #/usr/local/include/fftw3.h # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/fft.cpp -I/usr/local/include $(FFT) -o build/fft.o

build/visualizer.o: src/visualizer.h src/visualizer.cpp /usr/local/include/opencv4/opencv2/opencv.hpp /usr/include/opencv4/opencv2/core/mat.hpp /usr/include/opencv4/opencv2/core.hpp
	$(CC) $(OPTIONS) -c src/visualizer.cpp $(IOPENCV) -o build/visualizer.o $(LOPENCV)

clean:
	rm -f build/*.o test logs/*