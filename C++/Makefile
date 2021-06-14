CC=g++
CC_FLAGS=-lm -Wall -fprofile-arcs -ftest-coverage --coverage
INCS=-I/usr/lib -I/usr/include -I./include
GTEST=-lgtest -lgtest_main -pthread

all: fibheap_test graph_test trie_test

gcov:
	gcov *.o

cov: gcov
	lcov -c --directory . --output-file cov.info
	lcov --remove cov.info -o cov_filtered.info \
		"*_test.cpp" \
		"/usr/include/*"
	genhtml cov_filtered.info --output-directory out

src/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c $(<) -o $(*).o $(INCS)

trie_test: trie_test.cpp src/trie.o
	$(CC) $(<) trie.o -o $(@) $(GTEST) $(INCS) $(CC_FLAGS)

graph_test: graph_test.cpp src/graph.o src/fibheap.o
	$(CC) $(<) graph.o fibheap.o -o $(@) $(GTEST) $(INCS) $(CC_FLAGS)

fibheap_test: fibheap_test.cpp src/fibheap.o
	$(CC) $(<) fibheap.o -o $(@) $(GTEST) $(INCS) $(CC_FLAGS)

clean:
	rm -rf *.gch *.o *.gcov *.gcno *.gcda *_test *.info out/
	clear
	clear

