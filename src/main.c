#include <stdio.h>
#include <stdlib.h>
#include "log.h"
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
  log_init();

  // Specify stencil properties
  const Stencil stencil = {
    .size = STENCIL_SIZE,
    .range = STENCIL_RANGE,
    .type = VON_NEUMANN,
    .operation = SUM,
  };

  if (argc != 3) {
    printf("Usage: ./stencil <input> <output>\n");
    exit(1);
  }

  Matrix matrix = read_file(argv[1]);
  //print_matrix(matrix);
  
  // Compute specified number of iterations
  matrix = compute(matrix, stencil);

  //print_matrix(matrix);
  write_file(matrix, argv[2]);

  log_cleanup();
  free(matrix.data);
  return 0;
}

