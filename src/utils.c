#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "common.h"
#include "stdarg.h"
#include "utils.h"

void calculateIndices(Matrix matrix, uint32_t* indices, uint32_t offset) {
  uint32_t* multipliers = matrix.submatrix_sizes;

  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t multiplier = multipliers[i];

    indices[i] = offset / multiplier; // Get the index for the current dimension
    offset %= multiplier; // Update offset for the next dimension
  }
}

// Given the sumatrix depth, calculate its offset within the contiguous array
uint32_t calculateOffset(Matrix matrix, uint32_t depth) {
  uint32_t offset = 1;
  for (uint32_t i = depth + 1; i < matrix.dimensions; i++) {
    offset *= matrix.sizes[i];
  }
  return offset;
}

CELL_TYPE* getRef(Matrix matrix, va_list ptr) {
  // Calculate the index based on the sizes and provided indices
  uint32_t index = 0;
  uint32_t* multipliers = matrix.submatrix_sizes;

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

void printMatrixRec(Matrix matrix, uint32_t depth) {
  if (depth < matrix.dimensions) {
    printf("[");

    for (uint32_t i = 0; i < matrix.sizes[depth]; i++) {
      uint32_t offset = calculateOffset(matrix, depth);

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

