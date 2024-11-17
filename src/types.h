#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "common.h"

// Stencil types
typedef enum {
  MOORE,
  VON_NEUMANN,
} StencilType;

// Operation types
typedef enum {
  AVG, // Average
  WAVG, // Weighted average
  SUM, // Sum
  WSUM, // Weighted sum
} OperationType;

// Stencil structure
typedef struct {
  uint32_t size;
  StencilType type;
  OperationType operation;
} Stencil;

// Matrix structure
typedef struct {
  uint32_t total_size;
  uint32_t dimensions;
  uint32_t sizes[MAX_DIMS];
  uint32_t submat_sizes[MAX_DIMS];
  float* data;
} Matrix;

// Neighbor structure
typedef struct {
  float value;
  float weight;
} Neighbor;

#endif
