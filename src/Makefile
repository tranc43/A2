# ==== 
# CMPT 201 Assignment 2 Make file
# name: Christian Tran
# name: Brice Matias
# Contains appGame build
# clean target
# mem_valgrind target
# debug target
# doc target

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lncurses

SRCS = main.c splash.c level_select.c level1.c level2.c
OBJ = $(SRCS:.c=.o)

BIN_RELEASE = ../bin/appGame
BIN_DEBUG = ../bin/appGame_debug

.PHONY: all clean mem_valgrind debug doc

# app game target
all: $(BIN_RELEASE)

$(BIN_RELASE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(BIN_RELASE)
	@echo "Completed -> $(BIN_RELEASE)"

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

# clean target
clean:
	rm -f $(OBJ) $(BIN_RELASE) $(BIN_DEBUG)
	@echo "Cleaned"
# mem_valgrind target

mem_valgrind: $(BIN_RELASE)
	valgrind --leak-check=full --show-leak-kinds=all $(BIN_RELASE)


# debug target
debug: CFLAGS += -g -DDEBUG
debug: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(BIN_DEBUG)
	@echo "Debug build is finished -> $(BIN_DEBUG)"


# doc target
doc: 
	mkdir -p ../doc
	doxygen ../Doxyfile
	@echo "Documentation generated"
