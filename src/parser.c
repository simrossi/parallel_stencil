#include "parser.h"
#include "common.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix read_file(const char *filename) {
  Matrix matrix;
  char line[MAX_LINE_LENGTH];
  const char *delimiter = " ";
  FILE *file = fopen(filename, "r");

  if (!file) {
    fprintf(stderr, "Error: Failed to open input file '%s'.\n", filename);
    exit(1);
  }

  // Read and store dimensions sizes
  matrix.dimensions = 0;
  if (fgets(line, sizeof(line), file) != NULL) {
    char *token = strtok(line, delimiter);
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
    fprintf(stderr, "Error: Failed to allocate matrix data.\n");
    exit(1);
  }

  // Read and store data
  for (uint32_t i = 0; i < total_size; i++) {
    float value;
    if (fscanf(file, "%f", &value) == EOF) {
      fprintf(stderr, "Error: Unsufficient number of values provided.\n");
      exit(1);
    }

    matrix.data[i] = value;
  }

  fclose(file);
  return matrix;
}

void write_file(const Matrix matrix, const char *filename) {
  FILE *file = fopen(filename, "w");

  if (!file) {
    fprintf(stderr, "Error: Failed to open output file '%s'.\n", filename);
    exit(1);
  }

  // Write dimensions sizes
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    const char *delimiter = (i != matrix.dimensions - 1 ? " " : "\n");
    fprintf(file, "%u%s", matrix.sizes[i], delimiter);
  }

  // Write data
  for (uint32_t i = 0; i < matrix.total_size; i++) {
    const char *delimiter = (i != matrix.total_size - 1 ? " " : "\n");
    fprintf(file, "%g%s", matrix.data[i], delimiter);
  }

  fclose(file);
}

Matrix read_binfile(const char *filename) {
  Matrix matrix;
  FILE *file = fopen(filename, "rb");

  if (!file) {
    fprintf(stderr, "Error: Failed to open input file '%s'.\n", filename);
    exit(1);
  }

  // Read number of dimensions
  fread(&matrix.dimensions, sizeof(uint32_t), 1, file);

  // Read and store dimensions sizes
  fread(matrix.sizes, sizeof(uint32_t), matrix.dimensions, file);

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
    fprintf(stderr, "Error: Failed to allocate matrix data.\n");
    exit(1);
  }

  // Read and store data
  uint32_t bytes_read = fread(matrix.data, sizeof(float), total_size, file);
  if (bytes_read < total_size) {
    fprintf(stderr, "Error: Unsufficient number of values provided.\n");
    exit(1);
  }

  fclose(file);
  return matrix;
}

void write_binfile(const Matrix matrix, const char *filename) {
  FILE *file = fopen(filename, "wb");

  if (!file) {
    fprintf(stderr, "Error: Failed to open output file '%s'.\n", filename);
    exit(1);
  }

  // Write number of dimensions
  fwrite(&matrix.dimensions, sizeof(uint32_t), 1, file);

  // Write dimensions sizes
  fwrite(matrix.sizes, sizeof(uint32_t), matrix.dimensions, file);

  // Write data
  fwrite(matrix.data, sizeof(float), matrix.total_size, file);

  fclose(file);
}
