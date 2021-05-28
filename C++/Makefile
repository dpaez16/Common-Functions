CC=g++
CC_FLAGS=-lm -Wall -O2

build: main.o graph.o
	$(CC) $(CC_FLAGS) -o test $(^)

main.o: main.cpp graph.h
	$(CC) $(CC_FLAGS) -c $(^)

graph.o: graph.h graph.cpp
	$(CC) $(CC_FLAGS) -c $(^)

clean:
	rm -f *.gch *.o test