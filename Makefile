PROGRAM=get-it
OBJECTS=src/get_it.c src/parser.c
INCLUDE=src/parser.h
CC=gcc
LDFLAGS=-lcurl
DEBUG=-DDEBUG

.PHONY: $(PROGRAM)
$(PROGRAM):
	$(CC) $(OBJECTS) $(INCLUDE) $(LDFLAGS) -o $(PROGRAM)

debug:
	$(CC) $(OBJECTS) $(INCLUDE) $(LDFLAGS) $(DEBUG) -o $(PROGRAM)

run:
	make
	./$(PROGRAM)

.PHONY: clean
clean:
	-rm -f src/*.o
	-rm -f get-it
