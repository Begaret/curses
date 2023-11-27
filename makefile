CC=cc
CFLAGS=-Wall -Wextra -Ofast

SRC=$(filter-out src/curses_%.c, $(wildcard src/*.c))
OBJ=$(patsubst src/%.c, int/%.o, $(SRC))

GL=0
ifeq ($(GL), 1)
	LIB=-lSDL2
	CFLAGS:=$(CFLAGS) -DGL
else
	LIB=-lncurses
endif

OUT=bin/main

.PHONY: all clean

int/%.o: src/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $^ -o $@ $(LIB)

clean:
	rm -f int/*

