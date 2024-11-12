#include <math.h>
#include <stdlib.h>

#include "../common.h"
#include "../matrix.h"
#include "../types.h"
#include "../stencil.h"
#include "sum.h"

void sum(Matrix old_mat, Matrix new_mat, uint32_t offset, uint32_t range) {
  CELL_TYPE sum = 0;
  uint32_t indices[MAX_DIM];
  calculateIndices(old_mat, offset, indices);

  uint32_t cell_num;
  uint32_t max_cells = pow(2 * range + 1, old_mat.dimensions);
  Cell* cells = malloc(max_cells * sizeof(Cell));

  moore(old_mat, indices, range, cells, &cell_num);
  for (uint32_t i = 0; i < cell_num; i++) {
    sum += cells[i].value;
  }

  new_mat.buffer[offset] = sum;
  free(cells);
}
