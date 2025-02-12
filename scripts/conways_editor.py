#!/usr/bin/env python3
import pygame
import sys

pygame.init()

WINDOW_SIZE = 800
CELL_SIZE = 15
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

screen = pygame.display.set_mode((WINDOW_SIZE, WINDOW_SIZE))
pygame.display.set_caption("Conways Game Of Life Editor")

rows = cols = 0
matrix = []

def load(file):
    with open(file, "r") as inputfile:
        global rows, cols
        rows, cols = map(int, inputfile.readline().split())

        values = list(map(float, inputfile.readline().split()))

        for i in range(0, len(values), cols):
            row = [val != 0 for val in values[i : i + cols]]
            matrix.append(row)

def save(file):
    with open(file, "w") as outputfile:
        outputfile.write(f"{rows} {cols}\n")

        values = []
        for row in matrix:
            values.extend("1" if cell else "0" for cell in row)
        outputfile.write(" ".join(values))

def draw_matrix():
    screen.fill(WHITE)

    for row in range(rows):
        for col in range(cols):
            x = col * CELL_SIZE
            y = row * CELL_SIZE

            color = BLACK if matrix[row][col] else WHITE
            pygame.draw.rect(screen, color, (x, y, CELL_SIZE, CELL_SIZE))
            pygame.draw.rect(screen, BLACK, (x, y, CELL_SIZE, CELL_SIZE), 1)

    pygame.display.flip()

def get_cell_position(mouse_pos):
    x, y = mouse_pos

    # Check if click is within the grid
    if 0 <= x <= WINDOW_SIZE and 0 <= y <= WINDOW_SIZE:
        col = x // CELL_SIZE
        row = y // CELL_SIZE

        if 0 <= row < rows and 0 <= col < cols:
            return row, col
    return None

def main():
    if len(sys.argv) != 2:
        print("Usage: ./conways_editor.py <filename>")
        exit(0)

    file = sys.argv[1]
    load(file)

    try:
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    raise SystemExit

                if event.type == pygame.MOUSEBUTTONDOWN:
                    cell_pos = get_cell_position(pygame.mouse.get_pos())
                    if cell_pos:
                        row, col = cell_pos
                        matrix[row][col] = not matrix[row][col]

            draw_matrix()
    except SystemExit:
        save(file)
    finally:
        pygame.quit()
        sys.exit()

if __name__ == "__main__":
    main()