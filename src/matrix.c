#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "matrix.h"

void calculateIndices(Matrix matrix, uint32_t offset, uint32_t* indices) {
  uint32_t* multipliers = matrix.submatrix_sizes;

  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t multiplier = multipliers[i];

    indices[i] = offset / multiplier; // Get the index for the current dimension
    offset %= multiplier; // Update offset for the next dimension
  }
}

uint32_t calculateOffset(Matrix matrix, uint32_t* indices) {
  uint32_t offset = 0;

  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t index = indices[i];

    // Check for out-of-bounds access
    if (index >= matrix.sizes[i]) {
      fprintf(stderr, "Index out of bounds\n");
      exit(1);
    }

    offset += index * matrix.submatrix_sizes[i];
  }

  return offset;
}

CELL_TYPE get(Matrix matrix, ...) {
  va_list ptr;
  uint32_t indices[MAX_DIM];

  va_start(ptr, matrix);
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    indices[i] = va_arg(ptr, uint32_t);
  }
  va_end(ptr);

  uint32_t offset = calculateOffset(matrix, indices);
  return matrix.buffer[offset];
}

CELL_TYPE* getRef(Matrix matrix, ...) {
  va_list ptr;
  uint32_t indices[MAX_DIM];

  va_start(ptr, matrix);
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    indices[i] = va_arg(ptr, uint32_t);
  }
  va_end(ptr);

  uint32_t offset = calculateOffset(matrix, indices);
  return &matrix.buffer[offset];
}

CELL_TYPE getByIndex(Matrix matrix, uint32_t* indices) {
  uint32_t offset = calculateOffset(matrix, indices);
  return matrix.buffer[offset];
}

CELL_TYPE* getRefByIndex(Matrix matrix, uint32_t* indices) {
  uint32_t offset = calculateOffset(matrix, indices);
  return &matrix.buffer[offset];
}

void set(CELL_TYPE value, Matrix matrix, ...) {
  va_list ptr;
  uint32_t indices[MAX_DIM];

  va_start(ptr, matrix);
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    indices[i] = va_arg(ptr, uint32_t);
  }
  va_end(ptr);

  uint32_t offset = calculateOffset(matrix, indices);
  matrix.buffer[offset] = value;
}
