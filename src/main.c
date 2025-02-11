#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "log.h"
#include "operations.h"
#include "parallel.h"
#include "parser.h"
#include "patterns.h"
#include "sequential.h"
#include "stencil.h"
#include "types.h"
#include "utils.h"

#define PARALLEL // Perform parallel stencil computation

uint32_t num_threads = 1; // Number of threads used by OpenMP
char *save_intermediate = NULL;
bool binary = false;

int32_t main(int32_t argc, char **argv)
{
    char *input_file = NULL;
    char *output_file = NULL;
    char *log_file = NULL;

    init(argc, argv, &input_file, &output_file, &log_file, &save_intermediate, &binary);
    log_init(log_file);

    Matrix matrix = binary ? read_binfile(input_file) : read_file(input_file);
    Stencil stencil = cross_2d(1);

    init_stencil(stencil, avg);

#ifndef PARALLEL
    matrix = compute_sequential(matrix);
#else
    matrix = compute_parallel(matrix);
#endif

    // print_matrix(matrix);
    binary ? write_binfile(matrix, output_file) : write_file(matrix, output_file);

    log_cleanup();
    stencil_cleanup();
    free(matrix.data);
    return 0;
}
