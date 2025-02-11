#!/usr/bin/env python3
import struct
import sys


def main():
    if len(sys.argv) != 3:
        print("Usage: ./to_binary.py <text_input> <binary_output>")

    text_input = sys.argv[1]
    binary_output = sys.argv[2]

    dimensions = total_size = 0
    sizes = data = []

    with open(text_input, "r") as file:
        # Read dimensions
        line = file.readline().strip()
        sizes = [int(size) for size in line.split()]
        dimensions = len(sizes)

        total_size = 1
        for i in range(dimensions):
            total_size *= sizes[i]

        # Read data
        line = file.readline().strip()
        data = [float(value) for value in line.split()]

    with open(binary_output, "wb") as file:
        # Write number of dimensions
        file.write(struct.pack("I", dimensions))  # I: 32-bit unsigned integer

        # Write dimensions' sizes
        for i in range(dimensions):
            file.write(struct.pack("I", sizes[i]))  # I: 32-bit unsigned integer

        # Write data
        for i in range(total_size):
            file.write(struct.pack("f", data[i]))  # f: 32-bit float


if __name__ == "__main__":
    main()
