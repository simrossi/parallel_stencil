#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#define INTERMEDIATE_NAME_DIM 200//max file's name length
                                 //for intermediate results

#include "types.h"

/**
 * executes the sequential computation of the ISL
 * @param matrix data structure that represents the matrix used in ISL
 * @param iterations number of iterations to compute
 * @return the new matrix obtained from stencil iteration
 */
Matrix compute_sequential(const Matrix matrix, const uint32_t iterations);

#endif
