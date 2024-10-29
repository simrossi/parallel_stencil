CC:=gcc
CFLAGS:=-Wall -Wextra

SRC_DIR:=src
BUILD_DIR:=build

SRCS:=$(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

EXEC:=stencil
INPUT:=input
OUTPUT:=output

all: $(BUILD_DIR) $(EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXEC) $(INPUT) $(OUTPUT)

clean:
	rm -rf $(BUILD_DIR) $(EXEC)

.PHONY: all run clean
