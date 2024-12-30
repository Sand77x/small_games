CC = gcc
SRC := $(shell find src -name '*.c')
SRC_STRIPPED = $(SRC:src/%=%)
OBJ = $(SRC_STRIPPED:%.c=build/%.o)
CFLAGS = -Wextra -pedantic
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
INCLUDES = -I./include
LIBS = -L./lib
OUT = program

default: all

clean:
	rm -f $(OUT)
	rm -rf build/*

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(INCLUDES) $(LIBS) $(LDFLAGS) -o $(OUT)

build/%.o: src/%.c
	$(CC) -c $< $(CFLAGS) $(INCLUDES) $(LIBS) $(LDFLAGS) -o $@
