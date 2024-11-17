#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "parser.h"
#include "types.h"

Matrix read_file(const char* filename);
void write_file(const Matrix matrix, const char* filename);

Matrix read_file(const char* filename) {
  Matrix matrix;
  char line[MAX_LINE_LENGTH];
  const char* delimiter = " ";
  FILE* file = fopen(filename, "r");

  if (!file) {
    fprintf(stderr, "Error opening input file: %s\n", filename);
    exit(1);
  }

  // Read and store dimensions sizes
  matrix.dimensions = 0;
  if (fgets(line, sizeof(line), file) != NULL) {
    char* token = strtok(line, delimiter);
    while (token != NULL && matrix.dimensions < MAX_DIMS) {
      matrix.sizes[matrix.dimensions++] = atoi(token);
      token = strtok(NULL, delimiter);
    }
  }

  // Compute total number of elements and submatrices sizes
  uint32_t total_size = 1;
  for (int32_t i = matrix.dimensions - 1; i >= 0; i--) {
    matrix.submat_sizes[i] = total_size;
    total_size *= matrix.sizes[i];
  }

  // Allocate contiguous data buffer
  matrix.total_size = total_size;
  matrix.data = malloc(total_size * sizeof(float));
  if (matrix.data == NULL) {
    fprintf(stderr, "Error while allocating matrix data\n");
    exit(1);
  }

  // Read and store data
  for (uint32_t i = 0; i < total_size; i++) {
    float value;
    if (fscanf(file, "%f", &value) == EOF) {
      fprintf(stderr, "Unsufficient number of values provided\n");
      exit(1);
    }

    matrix.data[i] = value;
  }

  fclose(file);
  return matrix;
}

void write_file(const Matrix matrix, const char* filename) {
  FILE* file = fopen(filename, "w");

  if (!file) {
    fprintf(stderr, "Error opening output file: %s\n", filename);
    exit(1);
  }

  // Write dimensions sizes
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    const char* delimiter = (i != matrix.dimensions - 1 ? " " : "\n");
    fprintf(file, "%d%s", matrix.sizes[i], delimiter);
  }

  // Write data
  for (uint32_t i = 0; i < matrix.total_size; i++) {
    const char* delimiter = (i != matrix.total_size - 1 ? " " : "\n");
    fprintf(file, "%f%s", matrix.data[i], delimiter);
  }

  fclose(file);
}
