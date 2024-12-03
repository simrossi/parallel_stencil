#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "log.h"
#include "sequential.h"
#include "stencil.h"
#include "timer.h"
#include "types.h"

Matrix compute(const Matrix matrix) {
    Matrix tmp = matrix;

    // Allocate temporary matrix data buffer
    tmp.data = malloc(matrix.total_size * sizeof(float));

    double exec_time = 0;
    for (uint32_t i = 0; i < ITERATIONS; i++) {
        clock_t timer = init_timer();

        // For every element calculate the new value
        for (uint32_t i = 0; i < matrix.total_size; i++) {
            tmp.data[i] = compute_stencil(matrix, i);
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