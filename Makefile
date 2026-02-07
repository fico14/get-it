PROGRAM=get-it
OBJECTS=src/get_it.c src/parser.c src/list.c
CC=gcc
CFLAGS=-Wall -Wextra -Werror
LDFLAGS=-lcurl
DEBUG=-DDEBUG
INSTALL_DIR=~/.local/bin/

.PHONY: $(PROGRAM)
$(PROGRAM):
	$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM)

debug:
	$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) $(DEBUG) -o $(PROGRAM)

run:
	make
	@./$(PROGRAM)

install:
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir $(INSTALL_DIR); fi
	@chmod +x $(PROGRAM)
	@cp $(PROGRAM) $(INSTALL_DIR)
	@echo "get-it installed to $(INSTALL_DIR)"

.PHONY: clean
clean:
	-rm -f src/*.o
	-rm -f get-it
