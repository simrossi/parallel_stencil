#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "parser.h"

Matrix readfile(const char* filename) {
  Matrix matrix;

  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file: %s\n", filename);
    exit(1);
  }

  char line[LINE_DIM];
  const char* delimiter = " ";

  // Read first line and store the dimensions sizes
  matrix.dimensions = 0;
  if (fgets(line, sizeof(line), file) != NULL) {
    char* token = strtok(line, delimiter);
    while (token != NULL && matrix.dimensions < MAX_DIM) {
      matrix.sizes[(matrix.dimensions)++] = atoi(token);
      token = strtok(NULL, delimiter);
    }
  }

  // Count the total number of cells
  uint32_t size = 1;
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    size *= matrix.sizes[i];
  }

  // Allocate contiguous array of cells
  matrix.buffer = malloc(size * sizeof(CELL_TYPE));
  if (matrix.buffer == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }

  CELL_TYPE value;
  for (uint32_t i = 0; i < size && fscanf(file, "%d", &value) == 1; i++) {
    matrix.buffer[i] = value;
  }

  fclose(file);
  return matrix;
}

void writefile(const char* filename, Matrix matrix) {
  FILE* file = fopen(filename, "w");
  if (!file) {
    fprintf(stderr, "Failed to open file: %s\n", filename);
    exit(1);
  }

  uint32_t cell_num = 1;
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    cell_num *= matrix.sizes[i];

    const char* delimiter = (i != matrix.dimensions - 1 ? " " : "\n");
    fprintf(file, "%d%s", matrix.sizes[i], delimiter);
  }

  for (uint32_t i = 0; i < cell_num; i++) {
    const char* delimiter = (i != cell_num - 1 ? " " : "\n");
    fprintf(file, "%d%s", matrix.buffer[i], delimiter);
  }

  fclose(file);
}
