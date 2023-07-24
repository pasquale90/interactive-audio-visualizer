CC :=g++ -std=c++11 #gcc -std=c99 
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
OBJECTS :=build/fft.o build/audio.o build/main.o 
JACK :=-L/usr/lib/x86_64-linux-gnu/ -ljack -ljackserver # JACK := -L/usr/lib64 -ljack -ljackserver
FFT :=-lfftw3

runnable: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) $(JACK) $(FFT) -o test

build/main.o: src/main.cpp # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/main.cpp -I/usr/include $(JACK) $(FFT) -o build/main.o 

build/audio.o: src/audio.h src/audio.h #/usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/audio.cpp -I/usr/include $(JACK) -o build/audio.o

build/fft.o: src/fft.cpp #/usr/local/include/fftw3.h # /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/fft.cpp -I/usr/local/include $(FFT) -o build/fft.o  

clean:
	rm -f build/*.o test logs/*