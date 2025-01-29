#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "types.h"
#include "utils.h"

extern uint32_t num_threads;

void print_matrix_rec(const Matrix matrix, const uint32_t depth);

// Initialize program and parse provided arguments
void init(const int32_t argc, char **argv, char **input_file, char **output_file, char **log_file, bool *binary)
{
    *input_file = NULL;
    *output_file = NULL;
    *log_file = NULL;
    *binary = false;

    // Define the long options
    struct option long_options[] = {
        {"input-file", required_argument, 0, 'i'},
        {"output-file", required_argument, 0, 'o'},
        {"log-file", required_argument, 0, 'l'},
        {"threads", required_argument, 0, 't'},
        {"binary", no_argument, 0, 'b'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0} // End of options
    };

    while (true)
    {
        int32_t option_index = 0;
        int32_t opt = getopt_long(argc, argv, "i:o:l:t:bh", long_options, &option_index);

        if (opt == -1) {
            break;
        }

        switch (opt)
        {
            case 'i':
                *input_file = optarg;
                break;
            case 'o':
                *output_file = optarg;
                break;
            case 'l':
                *log_file = optarg;
                break;
            case 't':
                num_threads = atoi(optarg);
                break;
            case 'b':
                *binary = true;
                break;
            case 'h':
            case '?':
                // Print help message
                printf("Usage: program -i <input_file> -o <output_file> [-l <log_file>] [-t <num_threads>] [-b]\n");
                printf("Options:\n");
                printf("  -i, --input-file <file>   Specify the input file\n");
                printf("  -o, --output-file <file>  Specify the output file\n");
                printf("  -l, --log-file <file>     Specify the log file\n");
                printf("  -t, --threads <value>     Specify the number of threads\n");
                printf("  -b, --binary              Use binary format\n");
                printf("  -h, --help                Print this help message\n");
                exit(0);
            default:
                exit(1);
        }
    }

    // Check whether input file was provided
    if (*input_file == NULL)
    {
        fprintf(stderr, "Error: An input file must be specified. Use -i or --input-file.\n");
        exit(1);
    }

    // Check whether input file exists
    if (access(*input_file, R_OK) != 0)
    {
        fprintf(stderr, "Error: Input file '%s' does not exist.\n", *input_file);
        exit(1);
    }

    // Check whether output file was provided
    if (*output_file == NULL)
    {
        fprintf(stderr, "Error: An output file must be specified. Use -o or --output-file.\n");
        exit(1);
    }

    // Check whether extra arguments were provided
    if (optind < argc) {
        fprintf(stdout, "Warning: Extra arguments were provided:");
        while (optind < argc) {
            fprintf(stdout, " %s", argv[optind++]);
        }
        fprintf(stdout, "\n");
    }
}

void print_matrix(const Matrix matrix)
{
    print_matrix_rec(matrix, 0);
    printf("\n");
}

void print_matrix_rec(const Matrix matrix, const uint32_t depth)
{
    if (depth < matrix.dimensions)
    {
        printf("[");

        for (uint32_t i = 0; i < matrix.sizes[depth]; i++)
        {
            uint32_t offset = matrix.submat_sizes[depth];

            Matrix tmp = matrix;
            tmp.data += i * offset;
            print_matrix_rec(tmp, depth + 1);

            if (i != matrix.sizes[depth] - 1)
            {
                printf(",");
            }
        }

        printf("]");
    }
    else
    {
        printf("%.2f", *matrix.data);
    }
}

Matrix stencil_to_matrix(const Stencil stencil)
{
    Matrix matrix = {.dimensions = stencil.dimensions, .total_size = 1};

    memcpy(matrix.sizes, stencil.sizes, stencil.dimensions * sizeof(uint32_t));
    for (int32_t i = stencil.dimensions - 1; i >= 0; i--)
    {
        matrix.submat_sizes[i] = matrix.total_size;
        matrix.total_size *= matrix.sizes[i];
    }

    matrix.data = malloc(matrix.total_size * sizeof(float));
    memcpy(matrix.data, stencil.data, matrix.total_size * sizeof(float));

    return matrix;
}
