CC=perl

LIB=lib
TEST=test

LINT_FLAGS=-Mstrict -Mdiagnostics -cw
INCS=-I./$(LIB)

all: tests lint

tests: array_test hashmap_test string_test

%_test:
	$(CC) $(INCS) $(TEST)/$(@).pl

lint:
	$(CC) $(INCS) $(LINT_FLAGS) $(LIB)/FileIO.pm
	$(CC) $(INCS) $(LINT_FLAGS) $(LIB)/HashMap.pm
	$(CC) $(INCS) $(LINT_FLAGS) $(LIB)/HashSet.pm
	$(CC) $(INCS) $(LINT_FLAGS) $(LIB)/LinkedList.pm
	$(CC) $(INCS) $(LINT_FLAGS) $(LIB)/Queue.pm
	$(CC) $(INCS) $(LINT_FLAGS) $(LIB)/Stack.pm
	$(CC) $(INCS) $(LINT_FLAGS) $(LIB)/String.pm