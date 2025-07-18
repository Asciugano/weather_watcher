CC=gcc
CFLAGS=-Wall -std=c99 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -lm -ldl -lpthread -lX11 -framework OpenGL -lcurl -ljansson
BUILD_DIR=build/
SRC_DIR=src/
LIBS_DIR=$(SRC_DIR)lib/

SOURCES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIBS_DIR)*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
EXEC = $(BUILD_DIR)weather-watcher

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
