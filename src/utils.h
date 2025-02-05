#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include "types.h"

void init(const int32_t argc, char **argv, char **input_file, char **output_file, char **log_file, bool *binary);
void print_matrix(const Matrix matrix);

Matrix stencil_to_matrix(const Stencil stencil);

#endif
