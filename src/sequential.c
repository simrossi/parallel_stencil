#include "common.h"
#include "types.h"

void compute(Matrix matrix, void (*stencil)(Matrix, uint32_t, uint32_t), uint32_t steps) {
  for (uint32_t i = 0; i < steps; i++) {
    uint32_t num_cells = matrix.sizes[0] * matrix.submatrix_sizes[0]; // Total number of cells
                                                    //
    for (uint32_t j = 0; j < num_cells; j++) {
      stencil(matrix, j, STENCIL_SIZE);
    }
  }
}
