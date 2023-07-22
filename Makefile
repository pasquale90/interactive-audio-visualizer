CC :=g++ -std=c++11 #gcc -std=c99 
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
# COMPILE :=$(CC) $(NOPTIONS)
OBJECTS :=build/simple_client.o 
JACK := -L/usr/lib/x86_64-linux-gnu/ -ljack -ljackserver 
# JACK := -L/usr/lib64 -ljack -ljackserver

runnable: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) $(JACK) -o runnable

build/simple_client.o: src/simple_client2.cpp /usr/include/jack/jack.h /usr/include/jack/types.h
	$(CC) $(OPTIONS) -c src/simple_client2.cpp -I/usr/include $(JACK) -o build/simple_client.o 

clean:
	rm -f build/*.o runnable