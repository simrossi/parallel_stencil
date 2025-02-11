#include "common.h"
#include "log.h"
#include "parser.h"
#include "stencil.h"
#include "types.h"
#include <mpi.h>
#include <omp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t num_threads;
extern const char *save_intermediate;
extern const bool binary;

Matrix compute_parallel(const Matrix matrix) {
  Matrix tmp = matrix;
  int32_t procs, rank;

  // Initialize MPI
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Allocate starts, ends and counts arrays
  // Allocate starts, ends and counts arrays
  uint32_t size = procs * sizeof(int32_t);
  int32_t *starts = malloc(size);
  int32_t *ends = malloc(size);
  int32_t *counts = malloc(size);

  if (rank == 0) {
    // Allocate temporary matrix data buffer
    uint32_t total_bytes = matrix.total_size * sizeof(float);
    tmp.data = malloc(total_bytes);
    memcpy(tmp.data, matrix.data, total_bytes);

    // Divide the iteration across available processes
    uint32_t iters = matrix.total_size / procs;
    uint32_t remainder = matrix.total_size % procs;

    // Calculate starts, ends and counts arrays for every process
    // Calculate starts, ends and counts arrays for every process
    for (uint32_t i = 0; i < (uint32_t)procs; i++) {
      starts[i] = i * iters + (i < remainder ? i : remainder);
      ends[i] = starts[i] + iters + (i < remainder ? 1 : 0);
      counts[i] = ends[i] - starts[i];
    }
  }

  // Synchronize starts, ends and counts arrays
  // Synchronize starts, ends and counts arrays
  MPI_Bcast(starts, procs, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(ends, procs, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(counts, procs, MPI_INT, 0, MPI_COMM_WORLD);

  uint32_t start = starts[rank];
  uint32_t end = ends[rank];
  uint32_t count = counts[rank];

  // Allocate local buffer for every process
  float *local_data = malloc(count * sizeof(float));

  double exec_time = 0;
  for (uint32_t i = 0; i < ITERATIONS; i++) {
    double start_time = MPI_Wtime();

// For every element calculate the new value
#pragma omp parallel for num_threads(num_threads) schedule(guided)
    for (uint32_t j = start; j < end; j++) {
      local_data[j - start] = compute_stencil(tmp, j);
    }

    // Gather computed array data to the root process
    // Synchronize processes at the end of every iteration
    MPI_Allgatherv(local_data, count, MPI_FLOAT, tmp.data, counts, starts,
                   MPI_FLOAT, MPI_COMM_WORLD);

    if (rank == 0) {
      double time = MPI_Wtime() - start_time;
      exec_time += time;

      // Log current iteration execution time
      log_write("Iteration: %u, Time: %.6f seconds\n", i, time);

      // output intermediate iteration if save_intermediate is set to true
      if (save_intermediate) {
        char outputfile[256];

        sprintf(outputfile, "%s/iteration%u", save_intermediate, i);
        binary ? write_binfile(tmp, outputfile) : write_file(tmp, outputfile);
      }
    }
  }

  free(starts);
  free(ends);
  free(counts);
  free(local_data);
  MPI_Finalize();

  // After calling MPI_Finalize the number of process running is undefined
  // We return the value stored on the root process and exit the remaining
  // processes
  if (rank == 0) {
    free(matrix.data);
    log_write("Total execution time: %.6f seconds\n", exec_time);

    return tmp;
  }

  exit(0);
}
