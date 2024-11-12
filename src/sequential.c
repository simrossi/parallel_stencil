#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "sequential.h"
#include "types.h"

Matrix compute(Matrix matrix, void (*stencil)(Matrix, Matrix, uint32_t, uint32_t), uint32_t steps) {
  Matrix tmp = matrix;
  tmp.buffer = malloc(matrix.size * sizeof(CELL_TYPE));
  memcpy(tmp.buffer, matrix.buffer, matrix.size * sizeof(CELL_TYPE));

  for (uint32_t i = 0; i < steps; i++) {
    uint32_t num_cells = matrix.sizes[0] * matrix.submatrix_sizes[0]; // Total number of num_cells
    
    for (uint32_t j = 0; j < num_cells; j++) {
      stencil(matrix, tmp, j, STENCIL_SIZE);
    }
  }

  free(matrix.buffer);
  return tmp;
}
