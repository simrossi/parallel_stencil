#include <omp.h>
#include <stdint.h>
#include <stdlib.h>
#include "common.h"
#include "log.h"
#include "sequential.h"
#include "stencil.h"
#include "timer.h"
#include "types.h"

extern uint32_t num_threads;

Matrix compute_sequential(const Matrix matrix)
{
    Matrix tmp = matrix;

    // Allocate temporary matrix data buffer
    tmp.data = malloc(matrix.total_size * sizeof(float));

    double exec_time = 0;
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        clock_t timer = init_timer();

        // For every element calculate the new value
        #pragma omp parallel for num_threads(num_threads) schedule(guided)
        for (uint32_t j = 0; j < matrix.total_size; j++)
        {
            tmp.data[j] = compute_stencil(matrix, j);
        }

        double time = get_time(timer);
        exec_time += time;

        // Log current iteration execution time
        log_write("Iteration: %u, Time: %.6f seconds\n", i, time);
    }
    log_write("Total execution time: %.6f seconds\n", exec_time);

    free(matrix.data);
    return tmp;
}

