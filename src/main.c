#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "common.h"
#include "parser.h"
#include "types.h"
#include "utils.h"

#define ARG_SIZE 256
#define PARALLEL // Choose whether to perform stencil computations in parallel mode

#ifdef PARALLEL
#include "parallel.h"
#else
#include "sequential.h"
#endif

int32_t main(int32_t argc, char** argv) {
  if (argc != 3) {
    printf("Usage: ./stencil <input> <output>\n");
    exit(1);
  }

  Matrix matrix = readfile(argv[1]);

  printMatrix(matrix);
  writefile(argv[2], matrix);

  free(matrix.buffer);
	exit(0);
}
