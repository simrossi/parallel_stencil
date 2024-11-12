#include <math.h>
#include <stdlib.h>

#include "average.h"
#include "../common.h"
#include "../matrix.h"
#include "../types.h"
#include "../stencil.h"

void average(Matrix old_mat, Matrix new_mat, uint32_t offset, uint32_t range) {
  CELL_TYPE sum = 0;
  uint32_t indices[MAX_DIM];
  calculateIndices(old_mat, offset, indices);

  uint32_t num_cell;
  uint32_t max_cells = pow(2 * range + 1, old_mat.dimensions);
  Cell* cells = malloc(max_cells * sizeof(Cell));

  moore(old_mat, indices, range, cells, &num_cell);
  for (uint32_t i = 0; i < num_cell; i++) {
    sum += cells[i].value;
  }

  new_mat.buffer[offset] = sum / num_cell;
  free(cells);
}
