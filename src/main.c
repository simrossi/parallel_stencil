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

//TODO: calcolo dell'efficiency e del speed up (fare dei grafici) vedi slide 13 del corso

int32_t main(int32_t argc, char **argv)
{
    char *input_file = NULL;
    char *output_file = NULL;
    char *log_file = NULL;

    init(argc, argv, &input_file, &output_file, &log_file);
    log_init(log_file);

    Matrix matrix = read_file(input_file);
    Stencil stencil = vertical_edges_3x3();

    init_stencil(stencil, sum);

#ifndef PARALLEL
    matrix = compute_sequential(matrix);
#else
    matrix = compute_parallel(matrix);
#endif

    // print_matrix(matrix);
    write_file(matrix, output_file);

    log_cleanup();
    stencil_cleanup();
    free(matrix.data);
    return 0;
}
