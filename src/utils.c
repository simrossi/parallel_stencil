#include <stdio.h>
#include <stdint.h>

#include "utils.h"

void printMatrixRec(Matrix matrix, uint32_t depth) {
  if (depth < matrix.dimensions) {
    printf("[");

    for (uint32_t i = 0; i < matrix.sizes[depth]; i++) {
      uint32_t offset = matrix.submatrix_sizes[depth];

      Matrix tmp = matrix;
      tmp.buffer += i * offset;
      printMatrixRec(tmp, depth + 1);

      if (i != matrix.sizes[depth] - 1) {
        printf(",");
      }
    }

    printf("]");
  } else {
    printf("%d", *matrix.buffer);
  }
}

void printMatrix(Matrix matrix) {
  printMatrixRec(matrix, 0);
  printf("\n");
}

