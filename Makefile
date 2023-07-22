CC :=g++ -std=c++11 #gcc -std=c99 
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
# COMPILE :=$(CC) $(NOPTIONS)
OBJECTS :=build/audio_routing.o 
JACK := -L/usr/lib/x86_64-linux-gnu/ -ljack -ljackserver 
# JACK := -L/usr/lib64 -ljack -ljackserver

runnable: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) $(JACK) -o test

build/audio_routing.o: audio/routing.cpp /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c audio/routing.cpp -I/usr/include $(JACK) -o build/audio_routing.o 

clean:
	rm -f build/*.o test