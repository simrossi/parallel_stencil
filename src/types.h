#ifndef TYPES_H
#define TYPES_H

#include "common.h"
#include <stdint.h>

/**
 * Matrix structure
 * @param total_size size of contiguous buffer
 * @param dimensions matrix's number of dimensions
 * @param sizes list of the sizes of each dimension
 * @param submat_sizes number of data stored on each dimension
 * @param data pointer to contiguous buffer
 */
typedef struct {
  uint32_t total_size;
  uint32_t dimensions;
  uint32_t sizes[MAX_DIMS];
  uint32_t submat_sizes[MAX_DIMS];
  float *data;
} Matrix;

/**
 * Stencil structure
 * @param dimensions stencil's number of dimensions
 * @param sizes list of the sizes of each dimension
 * @param center list containing the center cell of each dimension
 * @param data pointer to stencil's data
 */
typedef struct {
  uint32_t dimensions;
  uint32_t sizes[MAX_DIMS];
  uint32_t center[MAX_DIMS];
  float *data;
} Stencil;

/**
 * Function pointer that represents an operation that processes an array of floats
 * @param data pointer to an array of values.
 * @param size number of elements in the array.
 * @return result computed from the given data.
 */
typedef float (*Operation)(const float *, const uint32_t);

#endif
