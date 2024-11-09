#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "matrix.h"
#include "utils.h"

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

void getArea(Matrix matrix, uint32_t* indices, uint32_t range, Cell* cells, uint32_t* cell_num) {
  uint32_t starts[MAX_DIM], ends[MAX_DIM], lengths[MAX_DIM], curr[MAX_DIM];

  *cell_num = range > 0;
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t index = indices[i];

    starts[i] = (index > range ? index - range : 0);
    ends[i] = (index + range < matrix.sizes[i] ? index + range : matrix.sizes[i] - 1);
    lengths[i] = (ends[i] - starts[i]) + 1;
    *cell_num *= lengths[i];
  }
  memcpy(curr, starts, sizeof(starts));

  for (uint32_t i = 0; i < *cell_num; i++) {
    Cell cell;
    cell.value = getByIndex(matrix, curr);
    memcpy(cell.indices, curr, matrix.dimensions * sizeof(uint32_t));
    cells[i] = cell;

    curr[matrix.dimensions - 1]++;
    for (uint32_t j = matrix.dimensions - 1; i != *cell_num - 1 && j > 0; j--) {
      if (curr[j] >= starts[j] + lengths[j]) {
        curr[j] = starts[j];
        curr[j - 1]++;
      }
    }
  }
}

/*void getCross(Matrix matrix, uint32_t* indices, uint32_t range, Cell* cells, uint32_t* cell_num) {
  // TODO
}*/

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
