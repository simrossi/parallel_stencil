#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * compute indices given an absolute offset
 * @param matrix data structure that represents the matrix used in ISL
 * @param offset element position in terms of absolute value
 * @param indices list of corresponding indices based on the absolute offset
 */
void offset_to_indices(const Matrix matrix, uint32_t offset, uint32_t *indices);

/**
 * compute absolute offset given the indices for every dimension
 * @param matrix data structure that represents the matrix used in ISL
 * @param indices element position inside the matrix defined as a set of indices
 * @return absolute offset of the element
 */
uint32_t indices_to_offset(const Matrix matrix, const uint32_t *indices);

/**
 * calculate the offset between two points
 * @param matrix data structure that represents the matrix used in ISL
 * @param center represent the stencils' centers
 * @param point actual absolute position inside the array
 * @param offsets the distance between the point and the center
 */
void calculate_offsets(const Matrix matrix, const uint32_t *center,
                       const uint32_t *point, int32_t *offsets);

/**
 * check whether the provided indices are out of bounds
 * @param matrix data structure that represents the matrix used in ISL
 * @param indices set of absolute positions 
 * @return True if at least one of the indices is outside the array space,
 * False otherwise
 */
bool is_outside(const Matrix matrix, const int32_t *indices);

/**
 * calculate closest indices belonging to the matrix
 * @param matrix data structure that represents the matrix used in ISL
 * @param indices set of absolute positions
 * @param closest_indices list containing the absolute position of the nearest
 * indices
 */
void closest_point(const Matrix matrix, const int32_t *indices,
                   uint32_t *closest_indices);

/**
 * given the indices of an element that belongs to the matrix, return the value stored
 * in the corresponding mono dimensional array
 * @param matrix data structure that represents the matrix used in ISL
 * @param ... variable number of indices
 * @return stored value in the defined indices
 */
float get(const Matrix matrix, ...);

/**
 * given the indices of an element that belongs to the matrix and the new value, set the 
 * new value in the corresponding mono dimensional array
 * @param matrix data structure that represents the matrix used in ISL
 * @param value new value to be stored
 * @param ... variable number of indices
 */
void set(const Matrix matrix, const float value, ...);

#endif
