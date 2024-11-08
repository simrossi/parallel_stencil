#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#include "common.h"

#define CELL_TYPE int32_t

typedef struct {
  uint32_t dimensions;
  uint32_t sizes[MAX_DIM];
  uint32_t submatrix_sizes[MAX_DIM]; // Hold the sizes of the submatrices indexed by their depth
  CELL_TYPE* buffer;
} Matrix;

#endif
