#!/usr/bin/env python3
import struct
import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: ./from_binary.py <binary_input> <text_output>")

    binary_input = sys.argv[1];
    text_output = sys.argv[2];

    dimensions = total_size = 0
    sizes = data = []

    with open(binary_input, 'rb') as file:
        # Read number of dimensions
        dimensions = struct.unpack('I', file.read(4))[0] # I: 32-bit unsigned integer

        # Read dimensions' sizes
        sizes = struct.unpack(f'{dimensions}I', file.read(dimensions * 4)) # I: 32-bit unsigned integer

        total_size = 1
        for i in range(dimensions):
            total_size *= sizes[i]

        # Read data
        data = struct.unpack(f'{total_size}f', file.read(total_size * 4)) # f: 32-bit float
        
    with open(text_output, 'w') as file:
        # Write dimensions
        file.write(' '.join(map(str, sizes)) + '\n')

        # Write data
        file.write(' '.join([f'{value:g}' for value in data]) + '\n')


if __name__ == "__main__":
    main()
