#include "sequential.h"
#include "common.h"
#include "log.h"
#include "parser.h"
#include "stencil.h"
#include "timer.h"
#include "types.h"
#include <omp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t num_threads;
extern const char *save_intermediate;
extern const bool binary;

Matrix compute_sequential(const Matrix matrix) {
  Matrix tmp = matrix;

  // Allocate temporary matrix data buffer
  uint32_t total_bytes = matrix.total_size * sizeof(float);
  tmp.data = malloc(total_bytes);

  double exec_time = 0;
  for (uint32_t i = 0; i < ITERATIONS; i++) {
    clock_t timer = init_timer();

    // For every element calculate the new value
    for (uint32_t j = 0; j < matrix.total_size; j++) {
      tmp.data[j] = compute_stencil(matrix, j);
    }

    double time = get_time(timer);
    exec_time += time;

    // Update new computed matrix
    memcpy(matrix.data, tmp.data, total_bytes);

    // Log current iteration execution time
    log_write("Iteration: %u, Time: %.6f seconds\n", i, time);

    // output intermediate iteration if save_intermediate is set to true
    if (save_intermediate) {
      char outputfile[256];

      sprintf(outputfile, "%s/iteration%u", save_intermediate, i);
      binary ? write_binfile(tmp, outputfile) : write_file(tmp, outputfile);
    }
  }
  log_write("Total execution time: %.6f seconds\n", exec_time);

  free(matrix.data);
  return tmp;
}
