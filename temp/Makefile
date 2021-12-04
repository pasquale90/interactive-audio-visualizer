CC=g++
FLAGS=-Wall
COMPILE=$(CC) $(FLAGS)
OBJECTS=obj/main.o obj/timer.o

run:$(OBJECTS)
	$(COMPILE) $(OBJECTS) -o run

obj/main.o:main.cpp timer.h
	$(COMPILE) -c main.cpp -I. -o obj/main.o

obj/timer.o:timer.h
	$(COMPILE) -c timer.cpp -o obj/timer.o
clean:
	-rm $(OBJECTS) run
