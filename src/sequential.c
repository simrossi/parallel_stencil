#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "log.h"
#include "matrix.h"
#include "stencil.h"
#include "timer.h"
#include "types.h"

// Stencils
#include "operations/average.h"
#include "operations/sum.h"

Matrix compute(const Matrix matrix, const Stencil stencil);
bool init(const Stencil stencil,
    uint32_t (**type)(const Matrix, const uint32_t*, const uint32_t, Neighbor*),
    float (**operation)(const Neighbor*, const uint32_t, const bool)
);

Matrix compute(const Matrix matrix, const Stencil stencil) {
  Matrix tmp = matrix;
  Matrix copy = matrix;

  // Allocate new matrices data buffer
  tmp.data = malloc(matrix.total_size * sizeof(float));
  copy.data = malloc(matrix.total_size * sizeof(float));
  memcpy(copy.data, matrix.data, matrix.total_size * sizeof(float));

  uint32_t (*type)(const Matrix, const uint32_t*, const uint32_t, Neighbor*);
  float (*operation)(const Neighbor*, const uint32_t, const bool);
  bool weighted = init(stencil, &type, &operation);

  const uint32_t max_cells = pow(stencil.size, matrix.dimensions);
  uint32_t indices[MAX_DIMS];
  Neighbor neighbors[max_cells];

  clock_t prog_timer = init_timer();
  for (uint32_t i = 0; i < ITERATIONS; i++) {
    clock_t iter_timer = init_timer();

    for (uint32_t i = 0; i < matrix.total_size; i++) {
      // For every cell get neighbors
      offset_to_indices(matrix, i, indices);
      const uint32_t num_cells = type(matrix, indices, stencil.size / 2, neighbors);

      // Compute specified operation
      tmp.data[i] = operation(neighbors, num_cells, weighted);
    }

    log_write("Iteration: %u, Time: %.6f seconds\n", i, get_time(iter_timer)); // Writing current iteration time to log file
    memcpy(copy.data, tmp.data, tmp.total_size * sizeof(float));
  }
  log_write("Total time: %.6f seconds\n", get_time(prog_timer)); // Writing total time to log file

  /// Free old matrix data buffer
  free(copy.data);
  free(matrix.data);
  return tmp;
}

bool init(const Stencil stencil,
    uint32_t (**type)(const Matrix, const uint32_t*, const uint32_t, Neighbor*),
    float (**operation)(const Neighbor*, const uint32_t, const bool)
) {
  bool weighted = false;

  // Select specified type of neighborhood
  switch (stencil.type) {
    case MOORE:
      *type = moore;
      break;
    case VON_NEUMANN:
      *type = von_neumann;
      break;
  }

  // Select specified operation
  switch (stencil.operation) {
    case WAVG:
      weighted = true;
      __attribute__ ((fallthrough));
    case AVG:
      *operation = average;
      break;
    case WSUM:
      weighted = true;
      __attribute__ ((fallthrough));
    case SUM:
      *operation = sum;
      break;
  }

  return weighted;
}
