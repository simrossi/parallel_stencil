#include <stdio.h>
#include "types.h"

void print_matrix(const Matrix matrix);
void print_matrix_rec(const Matrix matrix, const uint32_t depth);

void print_matrix(const Matrix matrix) {
  print_matrix_rec(matrix, 0);
  printf("\n");
}

void print_matrix_rec(const Matrix matrix, const uint32_t depth) {
  if (depth < matrix.dimensions) {
    printf("[");

    for (uint32_t i = 0; i < matrix.sizes[depth]; i++) {
      uint32_t offset = matrix.submat_sizes[depth];

      Matrix tmp = matrix;
      tmp.data += i * offset;
      print_matrix_rec(tmp, depth + 1);

      if (i != matrix.sizes[depth] - 1) {
        printf(",");
      }
    }

    printf("]");
  } else {
    printf("%.2f", *matrix.data);
  }
}

