CC=g++
CC_FLAGS=-lm -Wall -fprofile-arcs -ftest-coverage --coverage
GTEST=-lgtest -lgtest_main -pthread

SRC=src
INCLUDE=include
INCS=-I /usr/lib -I /usr/include -I $(INCLUDE)
TEST=test

OBJ=obj
BIN=bin
UTIL=util

all: $(patsubst $(INCLUDE)/%.h, %_test, $(wildcard $(INCLUDE)/*.h))

cov:
	bash $(UTIL)/get_cov.sh
	genhtml *.info --output-directory coverage

$(TEST)/%.o: $(TEST)/%.cpp
	mkdir -p $(OBJ) $(BIN)
	$(CC) $(CC_FLAGS) -c $(<) -o $(OBJ)/$(*).o $(INCS)

$(SRC)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ) $(BIN)
	$(CC) $(CC_FLAGS) -c $(<) -o $(OBJ)/$(*).o $(INCS)

trie_test: $(TEST)/trie_test.o $(SRC)/trie.o
	$(CC) $(patsubst %.o, $(OBJ)/%.o, $(notdir $(^))) -o $(BIN)/$(@) $(GTEST) $(INCS) $(CC_FLAGS)

graph_test: $(TEST)/graph_test.o $(SRC)/graph.o $(SRC)/fibheap.o
	$(CC) $(patsubst %.o, $(OBJ)/%.o, $(notdir $(^))) -o $(BIN)/$(@) $(GTEST) $(INCS) $(CC_FLAGS)

fibheap_test: $(TEST)/fibheap_test.o $(SRC)/fibheap.o
	$(CC) $(patsubst %.o, $(OBJ)/%.o, $(notdir $(^))) -o $(BIN)/$(@) $(GTEST) $(INCS) $(CC_FLAGS)

disjoint_set_test: $(TEST)/disjoint_set_test.o $(SRC)/disjoint_set.o $(SRC)/graph.o $(SRC)/fibheap.o
	$(CC) $(patsubst %.o, $(OBJ)/%.o, $(notdir $(^))) -o $(BIN)/$(@) $(GTEST) $(INCS) $(CC_FLAGS)

clean:
	rm -rf *.gch *.o *.gcov *.gcno *.gcda *_test *.info out/ obj/ bin/
	clear
	clear

