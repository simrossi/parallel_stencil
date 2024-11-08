#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void calculateIndices(Matrix matrix, uint32_t* indices, uint32_t offset);

CELL_TYPE get(Matrix matrix, ...);
void set(CELL_TYPE value, Matrix matrix, ...);
void copy(Matrix first, Matrix second);

void printMatrix(Matrix matrix);

#endif
