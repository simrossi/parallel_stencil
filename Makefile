CC:=mpicc
CFLAGS:=-std=c99 -Wall -Wextra -fopenmp
LDFLAGS:=-lm
SHELL:=/bin/bash
PROCS:=4
THREADS:=2

SRC_DIR:=src
BUILD_DIR:=build

SRCS:=$(shell find $(SRC_DIR) -name '*.c')
OBJS:= $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

EXEC:=stencil
INPUT_FILE:=input
OUTPUT_FILE:=output

all: $(BUILD_DIR) $(EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXEC): $(OBJS)
	module load mpi/openmpi-x86_64; \
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	module load mpi/openmpi-x86_64; \
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	module load mpi/openmpi-x86_64; \
	mpiexec -n $(PROCS) ./$(EXEC) -i $(INPUT_FILE) -o $(OUTPUT_FILE) -t $(THREADS)

clean:
	rm -rf $(BUILD_DIR) $(EXEC)

.PHONY: all run clean
