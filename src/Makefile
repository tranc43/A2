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
OBJS = $(SRCS:.c=.o)

BIN_DIR = ../bin
APP_NAME = appGame
BIN_RELEASE =  $(BIN_DIR)/$(APP_NAME)
BIN_DEBUG = $(BIN_DIR)/$(APP_NAME)_debug

.PHONY: all clean mem_valgrind debug doc

# appgame target

all: appGame
appGame: $(BIN_RELEASE)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_RELEASE): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	@echo "Build complete"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BIN_RELEASE) $(BIN_DEBUG)
	@echo "Cleaned object and files"

mem_valgrind: $(BIN_RELEASE)
	valgrind --leak-check=full --show-leak-kinds=all $(BIN_RELEASE)

debug: CFLAGS += -g -DDEBUG
debug: $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(BIN_DEBUG)
	@echo "Debug build complete"

doc:
	mkdir -p ../doc
	doxygen ../Doxyfile
	@echo "Documentation generated"
