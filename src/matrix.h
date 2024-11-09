#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"

void calculateIndices(Matrix matrix, uint32_t offset, uint32_t* indices);
uint32_t calculateOffset(Matrix matrix, uint32_t* indices);

void getArea(Matrix matrix, uint32_t* indices, uint32_t range, Cell* cells, uint32_t* num_cell);
//void getCross(Matrix matrix, uint32_t* indices, uint32_t range, Cell* cells, uint32_t* num_cell);

CELL_TYPE get(Matrix matrix, ...);
CELL_TYPE* getRef(Matrix matrix, ...);
CELL_TYPE getByIndex(Matrix matrix, uint32_t* indices);
CELL_TYPE* getRefByIndex(Matrix matrix, uint32_t* indices);
void set(CELL_TYPE value, Matrix matrix, ...);

#endif
