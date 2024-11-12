#include <string.h>

#include "matrix.h"
#include "stencil.h"
#include "types.h"

void moore(Matrix matrix, uint32_t* indices, uint32_t range, Cell* cells, uint32_t* num_cell) {
  uint32_t starts[MAX_DIM], ends[MAX_DIM], lengths[MAX_DIM], curr[MAX_DIM];

  *num_cell = range > 0;
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t index = indices[i];

    starts[i] = (index > range ? index - range : 0);
    ends[i] = (index + range < matrix.sizes[i] ? index + range : matrix.sizes[i] - 1);
    lengths[i] = (ends[i] - starts[i]) + 1;
    *num_cell *= lengths[i];
  }
  memcpy(curr, starts, sizeof(starts));

  for (uint32_t i = 0; i < *num_cell; i++) {
    Cell cell;
    cell.value = getByIndex(matrix, curr);
    memcpy(cell.indices, curr, matrix.dimensions * sizeof(uint32_t));
    cells[i] = cell;

    curr[matrix.dimensions - 1]++;
    for (uint32_t j = matrix.dimensions - 1; i != *num_cell - 1 && j > 0; j--) {
      if (curr[j] >= starts[j] + lengths[j]) {
        curr[j] = starts[j];
        curr[j - 1]++;
      }
    }
  }
}

void vonNeumann(Matrix matrix, uint32_t* indices, uint32_t range, Cell* cells, uint32_t* num_cell) {
  // TODO
}
