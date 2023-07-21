CC :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
# COMPILE :=$(CC) $(NOPTIONS)
OBJECTS :=build/main.o build/tempsignal.o

runnable: $(OBJECTS) #runnable
	$(CC) $(OPTIONS) $(OBJECTS) -o runnable

build/main.o: src/main.cpp src/tempsignal.h
	$(CC) $(OPTIONS) -c src/main.cpp -Ilibs/AudioFile -Ibuild -o build/main.o

build/tempsignal.o: src/tempsignal.h
	$(CC) $(OPTIONS) -c src/tempsignal.cpp -Ilibs/AudioFile -o build/tempsignal.o

clean:
	rm -f build/*.o runnable
















