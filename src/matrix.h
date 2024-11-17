#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"

void offset_to_indices(const Matrix matrix, uint32_t offset, uint32_t* indices);
uint32_t indices_to_offset(const Matrix matrix, const uint32_t* indices);
float get(const Matrix matrix, ...);
void set(const Matrix matrix, const float value, ...);

#endif
