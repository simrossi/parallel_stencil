#include <mpi.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "log.h"
#include "sequential.h"
#include "stencil.h"
#include "types.h"

Matrix compute_parallel(const Matrix matrix)
{
    Matrix tmp = matrix;
    int32_t procs, rank;

    // Initialize MPI
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Allocate starts, ends, counts and displacement arrays
    uint32_t size = procs * sizeof(int32_t);
    int32_t *starts = malloc(size);
    int32_t *ends = malloc(size);
    int32_t *counts = malloc(size);
    int32_t *displs = malloc(size);

    if (rank == 0) {
        // Allocate temporary matrix data buffer
        tmp.data = malloc(matrix.total_size * sizeof(float));

        // Divide the iteration across available processes
        uint32_t iters = matrix.total_size / procs;
        uint32_t remainder = matrix.total_size % procs;

        // Calculate starts, ends, counts and displacement arrays for every process
        uint32_t total_count = 0;
        for (uint32_t i = 0; i < (uint32_t)procs; i++) {
            starts[i] = i * iters + (i < remainder ? i : remainder);
            ends[i] = starts[i] + iters + (i < remainder ? 1 : 0);
            counts[i] = ends[i] - starts[i];
            displs[i] = total_count;
            total_count += counts[i];
        }
    }

    // Synchronize starts, ends, counts and displacement arrays
    MPI_Bcast(starts, procs, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(ends, procs, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(counts, procs, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(displs, procs, MPI_INT, 0, MPI_COMM_WORLD);

    uint32_t start = starts[rank];
    uint32_t end = ends[rank];
    uint32_t count = counts[rank];

    // Allocate local buffer for every process
    float *local_data = malloc(count * sizeof(float));

    double exec_time = 0;
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        double start_time = MPI_Wtime();

        // For every element calculate the new value
        #pragma omp parallel for num_threads(2) schedule(guided)
        for (uint32_t i = start; i < end; i++)
        {
            local_data[i - start] = compute_stencil(matrix, i);
        }

        // Gather computed array data to the root process
        // Synchronize processes at the end of every iteration
        MPI_Gatherv(local_data, count, MPI_FLOAT, tmp.data, counts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            double time = MPI_Wtime() - start_time;
            exec_time += time;

            // Log current iteration execution time
            log_write("Iteration: %u, Time: %.6f seconds\n", i, time);
        }
    }

    free(starts);
    free(ends);
    free(counts);
    free(displs);
    free(local_data);
    MPI_Finalize();

    // After calling MPI_Finalize the number of process running is undefined
    // We return the value stored on the root process and exit the remaining processes
    if (rank == 0) {
        free(matrix.data);
        log_write("Total execution time: %.6f seconds\n", exec_time);

        return tmp;
    }

    exit(0);
}

