#ifndef PARALLEL_H
#define PARALLEL_H

#define INTERMEDIATE_NAME_DIM 200//max file's name length
                                 //for intermediate results

#include "types.h"

/**
 * executes the parallel computation of the ISL
 * @param matrix data structure that represents the matrix used in ISL
 * @return the new matrix obtained from stencil iteration
 */
Matrix compute_parallel(const Matrix matrix);

#endif