CC:=gcc
CFLAGS:=-Wall -Wextra -fopenmp
LDFLAGS:=-lm

SRC_DIR:=src
BUILD_DIR:=build

SRCS:=$(shell find $(SRC_DIR) -name '*.c')
OBJS:= $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
OBJDIRS:=$(dir $(OBJS))

EXEC:=stencil
INPUT_FILE:=input
OUTPUT_FILE:=output

all: $(BUILD_DIR) $(EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJDIRS)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXEC) -i $(INPUT_FILE) -o $(OUTPUT_FILE)

clean:
	rm -rf $(BUILD_DIR) $(EXEC)

.PHONY: all run clean
