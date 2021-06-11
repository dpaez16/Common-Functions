CC=g++
CC_FLAGS=-lm -Wall -fprofile-arcs -ftest-coverage --coverage
INCS=-I./ -I../ -I/usr/lib -I/usr/include
GTEST=-lgtest -lgtest_main -pthread

all: fibheap_test graph_test trie_test

cov:
	gcov ../fibheap.o ../graph.o ../trie.o
	lcov -c --directory .. --output-file cov.info
	genhtml cov.info --output-directory out

.cpp.o:
	$(CC) $(CC_FLAGS) -c $< -o $(@) $(INCS)

trie_test: trie_test.cpp ../trie.o
	$(CC) $(^) -o $(@) $(GTEST) $(INCS) $(CC_FLAGS)

graph_test: graph_test.cpp ../graph.o ../fibheap.o
	$(CC) $(^) -o $(@) $(GTEST) $(INCS) $(CC_FLAGS)

fibheap_test: fibheap_test.cpp ../fibheap.o
	$(CC) $(^) -o $(@) $(GTEST) $(INCS) $(CC_FLAGS)

clean:
	rm -rf *.gch *.o *.gcov *.gcno *.gcda *_test *.info out/
	clear
	clear

