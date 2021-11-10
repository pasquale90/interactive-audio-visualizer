CC :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
# COMPILE :=$(CC) $(NOPTIONS)
OBJECTS :=main.o

all: $(OBJECTS) audio_visualizer
	$(CC) $(OPTIONS) $(OBJECTS)

main.o: main.cpp
	$(CC) $(OPTIONS) -c main.cpp -I libs/AudioFile

clean:
	rm -f *.o
















