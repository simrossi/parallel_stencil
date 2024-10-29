#ifndef TYPES_H
#define TYPES_H

#include "common.h"

#define CELL_TYPE int32_t

typedef struct {
  uint32_t dimensions;
  uint32_t sizes[MAX_DIM];
  CELL_TYPE* buffer;
} Matrix;

#endif
