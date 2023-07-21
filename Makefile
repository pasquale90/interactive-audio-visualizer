CC :=g++
OPTIONS :=-g -std=c++11 -pedantic -Wall -Wextra -Wno-error
# COMPILE :=$(CC) $(NOPTIONS)
OBJECTS :=build/main.o 
JACK := -L/usr/lib64 -ljack -ljackserver

runnable: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) -o runnable

build/main.o: src/simple_client.cpp /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/simple_client.cpp -Ibuild -o build/simple_client.o

clean:
	rm -f build/*.o runnable
















