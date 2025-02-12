#!/usr/bin/env python3
import re
import os
import sys
import glob
import numpy as np
from PIL import Image

def load(file):
    with open(file, "r") as inputfile:
        global rows, cols
        rows, cols = map(int, inputfile.readline().split())

        values = list(map(float, inputfile.readline().split()))

        matrix = []
        for i in range(0, rows * cols, cols):
            row = [val != 0 for val in values[i : i + cols]]
            matrix.append(row)

    return matrix

def matrix_to_image(matrix):
    # Normalize image to 0-255 range
    flat_list = [item for row in matrix for item in row]
    min_value = min(flat_list)
    max_value = max(flat_list)

    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            matrix[i][j] = int(
                (matrix[i][j] - min_value) / (max_value - min_value) * 255
            )

    # Create PIL Image
    matrix = np.array(matrix, dtype=np.uint8)
    return Image.fromarray(matrix, mode="L")

def main():
    argc = len(sys.argv)

    if argc < 2:
        print("Usage: ./create_gif.py <folder> [output]")
        exit(0)

    folder = sys.argv[1]
    output = output = sys.argv[2] if argc >= 3 else "output.gif"

    # Get all files in the folder and sort them by the last number contained in their name
    files = glob.glob(os.path.join(folder, "*"))

    def last_number(file_path):
        filename = os.path.basename(file_path)
        numbers = re.findall(r"\d+", filename)

        # Return the last number found
        return int(numbers[-1]) if numbers else -1

    files = sorted(files, key=last_number)

    frames = []
    for file_path in files:
        matrix = load(file_path)
        image = matrix_to_image(matrix)
        frames.append(image)

    frames[0].save(
        output,
        save_all=True,
        append_images=frames[1:],
        duration=200,
    )

if __name__ == "__main__":
    main()