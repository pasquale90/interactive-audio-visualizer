CC :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
# COMPILE :=$(CC) $(NOPTIONS)
OBJECTS :=build/main.o build/tempsignal.o

runnable: $(OBJECTS) runnable
	$(CC) $(OPTIONS) $(OBJECTS) -o runnable

build/main.o: src/main.cpp include/tempsignal.h
	$(CC) $(OPTIONS) -c src/main.cpp -Ilibs/AudioFile -Ibuild -Iinclude -o build/main.o

build/tempsignal.o: include/tempsignal.h
	$(CC) $(OPTIONS) -c src/tempsignal.cpp -Ilibs/AudioFile -Iinclude -o build/tempsignal.o

clean:
	rm -f build/*.o runnable
















