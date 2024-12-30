CC = gcc
CFLAGS = -Werror
LDFLAGS = -lmingw32 -lSDL2Main -lSDL2
INCLUDES = -IC:/mingwlibs/include
LIBS = -LC:/mingwlibs/lib

BUILD_PATH = build
SRC = main.c game.c init.c app.c
OBJ = $(SRC:%.c=$(BUILD_PATH)/%.o)
OUT = program

all: $(OUT) $(BUILD_PATH)

$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS)

$(BUILD_PATH)/%.o: %.c $(BUILD_PATH)
	$(CC) -c -o  $@ $< $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS)

$(BUILD_PATH):
	mkdir build
