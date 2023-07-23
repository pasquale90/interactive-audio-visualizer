CC :=g++ -std=c++11 
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
OBJECTS :=build/fft.o
FFT :=-lfftw3 #/usr/local/lib/libfftw3.a #-L/usr/local/lib -Wl,-Bstatic -lfftw3 -lm
# JACK := -L/usr/lib64 -ljack -ljackserver

test: $(OBJECTS)
	$(CC) $(OPTIONS) $(OBJECTS) $(FFT) -o test 
#$(JACK)

build/fft.o: visualizer/fft.cpp /usr/local/include/fftw3.h # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c visualizer/fft.cpp -I/usr/local/include $(FFT) -o build/fft.o  


#$(JACK)

clean:
	rm -f build/*.o test