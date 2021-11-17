CC :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Wno-error
# COMPILE :=$(CC) $(NOPTIONS)
OBJECTS :=main.o tempsignal.o

runnable: $(OBJECTS) runnable
	$(CC) $(OPTIONS) $(OBJECTS) -o runnable

main.o: main.cpp tempsignal.h
	$(CC) $(OPTIONS) -c main.cpp -Ilibs/AudioFile -Ibuild -o main.o

tempsignal.o: tempsignal.h
	$(CC) $(OPTIONS) -c tempsignal.cpp -Ilibs/AudioFile -o tempsignal.o

clean:
	rm -f *.o
















