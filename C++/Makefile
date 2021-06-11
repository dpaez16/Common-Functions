CC=g++
CC_FLAGS=-lm -Wall -O2

build: main.o graph.o fibheap.o trie.o
	$(CC) $(CC_FLAGS) -o test $(^)

main.o: main.cpp graph.h fibheap.h trie.h
	$(CC) $(CC_FLAGS) -c $(^)

fibheap.o: fibheap.h fibheap.cpp
	$(CC) $(CC_FLAGS) -c $(^)

graph.o: graph.h graph.cpp
	$(CC) $(CC_FLAGS) -c $(^)

trie.o: trie.h trie.cpp
	$(CC) $(CC_FLAGS) -c $(^)

clean:
	rm -f *.gch *.o *.gcno *.gcov *.gcda *.info test
	clear
	clear
