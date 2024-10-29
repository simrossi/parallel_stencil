#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "common.h"
#include "stdarg.h"
#include "utils.h"

CELL_TYPE* getRef(Matrix matrix, va_list ptr) {
  // Calculate the index based on the sizes and provided indices
  uint32_t index = 0;
  uint32_t multiplier = 1;

  uint32_t multipliers[matrix.dimensions];
  for (int32_t i = matrix.dimensions - 1; i >= 0; i--) {
    multipliers[i] = multiplier;
    multiplier *= matrix.sizes[i];
  }

  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t idx = va_arg(ptr, uint32_t);

    // Check for out-of-bounds access
    if (idx >= matrix.sizes[i]) {
      fprintf(stderr, "Index out of bounds\n");
      va_end(ptr);
      exit(1);
    }

    index += idx * multipliers[i];
  }

  return &matrix.buffer[index];
}

CELL_TYPE get(Matrix matrix, ...) {
  va_list ptr;
  va_start(ptr, matrix);
  CELL_TYPE* cell = getRef(matrix, ptr);
  va_end(ptr);

  return *cell;
}

void set(CELL_TYPE value, Matrix matrix, ...) {
  va_list ptr;
  va_start(ptr, matrix);
  CELL_TYPE* cell = getRef(matrix, ptr);
  va_end(ptr);

  *cell = value;
}

uint32_t calculateOffset(uint32_t dimensions, uint32_t* sizes, uint32_t depth) {
  uint32_t offset = 1;
  for (uint32_t i = depth + 1; i < dimensions; i++) {
    offset *= sizes[i];
  }
  return offset;
}

void printMatrixRec(Matrix matrix, uint32_t depth) {
  if (depth < matrix.dimensions) {
    printf("[");

    for (uint32_t i = 0; i < matrix.sizes[depth]; i++) {
      uint32_t offset = calculateOffset(matrix.dimensions, matrix.sizes, depth);

      Matrix tmp = matrix;
      tmp.buffer += i * offset;
      printMatrixRec(tmp, depth + 1);

      if (i != matrix.sizes[depth] - 1) {
        printf(",");
      }
    }

    printf("]");
  } else {
    printf("%d", *matrix.buffer);
  }
}

void printMatrix(Matrix matrix) {
  printMatrixRec(matrix, 0);
  printf("\n");
}

