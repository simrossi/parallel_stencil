#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "stencil.h"
#include "types.h"

// Stencils
#include "operations/average.h"
#include "operations/sum.h"

Matrix compute(const Matrix matrix, const Stencil stencil);

Matrix compute(const Matrix matrix, const Stencil stencil) {
  Matrix tmp = matrix;
  bool weighted = false;
  tmp.data = malloc(matrix.total_size * sizeof(float)); // Allocate new matrix data buffer

  // Select specified type of neighborhood
  uint32_t (*type)(const Matrix, const uint32_t*, const uint32_t, Neighbor*);
  switch (stencil.type) {
    case MOORE:
      type = moore;
      break;
    case VON_NEUMANN:
      type = von_neumann;
      break;
  }

  // Select specified operation
  float (*operation)(const Neighbor*, const uint32_t, const bool);
  switch (stencil.operation) {
    case WAVG:
      weighted = true;
      __attribute__ ((fallthrough));
    case AVG:
      operation = average;
      break;
    case WSUM:
      weighted = true;
      __attribute__ ((fallthrough));
    case SUM:
      operation = sum;
      break;
  }

  const uint32_t max_cells = pow(stencil.size, matrix.dimensions);
  uint32_t indices[MAX_DIMS];
  Neighbor neighbors[max_cells];

  for (uint32_t i = 0; i < matrix.total_size; i++) {
    // For every cell get neighbors
    offset_to_indices(matrix, i, indices);
    const uint32_t num_cells = type(matrix, indices, stencil.size / 2, neighbors);

    // Compute specified operation
    tmp.data[i] = operation(neighbors, num_cells, weighted);
  }

  free(matrix.data); // Free old matrix data buffer
  return tmp;
}

