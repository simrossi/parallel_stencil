#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * initialize program and parse provided arguments
 * @param argc number of arguments
 * @param argv list of arguments
 * @param input_file the input file name
 * @param output_file the output file name
 * @param log_file the output log name
 * @param iterations number of iterations to compute
 * @param save_intermediate folder location to save intermediate results
 * @param binary indicates if the input and output are in binary format
 */
void init(const int32_t argc, char **argv, char **input_file,
          char **output_file, char **log_file, uint32_t *iterations,
          char **save_intermediate, bool *binary);

/**
 * print the values stored in the matrix on the std output
 * @param matrix data structure that represents the matrix used in ISL
 */
void print_matrix(const Matrix matrix);

/**
 * convert the stencil into a matrix 
 * @param stencil represented with the Stencil data structure
 * @return stencil as a Matrix
 */
Matrix stencil_to_matrix(const Stencil stencil);

#endif
