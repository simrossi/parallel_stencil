#ifndef UTILS_H
#define UTILS_H

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "types.h"

void init(const int32_t argc, char** argv, char** input_file, char** output_file, char** log_file);
void print_matrix(const Matrix matrix);

Matrix stencil_to_matrix(const Stencil stencil);

#endif