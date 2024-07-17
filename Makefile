PROGRAM=get-it
OBJECTS=src/get_it.c
CC=gcc
LDFLAGS=-lcurl
DEBUG=-DDEBUG

.PHONY: $(PROGRAM)
$(PROGRAM):
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(PROGRAM)

debug:
	$(CC) $(OBJECTS) $(LDFLAGS) $(DEBUG) -o $(PROGRAM)

run:
	./$(PROGRAM)

.PHONY: clean
clean:
	-rm -f src/*.o
	-rm -f get-it
