#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "parser.h"
#include "types.h"
#include "utils.h"

// #define PARALLEL // Perform parallel stencil computation
#ifdef PARALLEL
#include "parallel.h"
#else
#include "sequential.h"
#endif

int32_t main(int32_t argc, char** argv) {
  // Specify stencil properties
  const Stencil stencil = {
    .size = STENCIL_SIZE,
    .type = MOORE,
    .operation = SUM,
  };

  if (argc != 3) {
    printf("Usage: ./stencil <input> <output>\n");
    exit(1);
  }

  Matrix matrix = read_file(argv[1]);
  print_matrix(matrix);
  
  // Compute specified number of iterations
  for (uint32_t i = 0; i < ITERATIONS; i++) {
    matrix = compute(matrix, stencil);
  }

  print_matrix(matrix);
  write_file(matrix, argv[2]);

  free(matrix.data);
  return 0;
}
