PROGRAM=get-it
OBJECTS=src/get_it.c src/parser.c src/list.c
#INCLUDE=src/parser.h
CC=gcc
LDFLAGS=-lcurl
DEBUG=-DDEBUG

.PHONY: $(PROGRAM)
$(PROGRAM):
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(PROGRAM)

debug:
	$(CC) $(OBJECTS) $(LDFLAGS) $(DEBUG) -o $(PROGRAM)

run:
	make
	./$(PROGRAM)

install:
	mkdir ~/.mybin
	chmod +x $(PROGRAM)
	cp $(PROGRAM) ~/.mybin

.PHONY: clean
clean:
	-rm -f src/*.o
	-rm -f get-it
