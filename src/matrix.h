#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stdint.h>
#include "types.h"

void offset_to_indices(const Matrix matrix, uint32_t offset, uint32_t *indices);
uint32_t indices_to_offset(const Matrix matrix, const uint32_t *indices);
void calculate_offsets(const Matrix matrix, const uint32_t *center, const uint32_t *point, int32_t *offsets);
bool is_outside(const Matrix matrix, const int32_t *indices);
void closest_point(const Matrix matrix, const int32_t *indices, uint32_t *closest_indices);
float get(const Matrix matrix, ...);
void set(const Matrix matrix, const float value, ...);

#endif
