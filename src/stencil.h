#ifndef STENCIL_H
#define STENCIL_H

#include "types.h"
#include <stdint.h>

/**
 * initialize the matrix data structure that will represnet the stencil
 * by defining it's shape and the operation it has to compute
 * @param stencil Stencil data structure that defines its dimension and values
 * @param operation type of operation it will compute on for each cell (e.g sum, avg)
 */
void init_stencil(const Stencil stencil, const Operation operation);

/**
 * free the allocated memory used to store the stencil
 */
void stencil_cleanup();

/**
 * apply the operation of the stencil on the defined cell of the matrix
 * @param matrix data structure that represents the matrix used in ISL
 * @param offset absolute position of the cell
 * @return the new cell value
 */
float compute_stencil(const Matrix matrix, const uint32_t offset);

#endif