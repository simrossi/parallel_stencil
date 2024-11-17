#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "types.h"

void offset_to_indices(const Matrix matrix, uint32_t offset, uint32_t* indices);
uint32_t indices_to_offset(const Matrix matrix, const uint32_t* indices);
float get(const Matrix matrix, ...);
void set(Matrix matrix, const float value, ...);

// Compute indices given an absolute offset
void offset_to_indices(const Matrix matrix, uint32_t offset, uint32_t* indices) {
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t submat_size = matrix.submat_sizes[i];

    indices[i] = offset / submat_size; // Get index for current dimension
    offset %= submat_size; // Update offset for next dimension
  }
}

// Compute absolute offset given the indices for every dimension
uint32_t indices_to_offset(const Matrix matrix, const uint32_t* indices) {
  uint32_t offset = 0;

  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    const uint32_t index = indices[i];

    if (index >= matrix.sizes[i]) {
      fprintf(stderr, "Index out of bounds\n");
      exit(1);
    }

    offset += index * matrix.submat_sizes[i];
  }

  return offset;
}

float get(const Matrix matrix, ...) {
  va_list ptr;
  uint32_t indices[MAX_DIMS] = {0};

  va_start(ptr, matrix);
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    indices[i] = va_arg(ptr, uint32_t);
  }
  va_end(ptr);

  const uint32_t offset = indices_to_offset(matrix, indices);
  return matrix.data[offset];
}

void set(const Matrix matrix, const float value, ...) {
  va_list ptr;
  uint32_t indices[MAX_DIMS] = {0};

  va_start(ptr, value);
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    indices[i] = va_arg(ptr, uint32_t);
  }
  va_end(ptr);

  const uint32_t offset = indices_to_offset(matrix, indices);
  matrix.data[offset] = value;
}

