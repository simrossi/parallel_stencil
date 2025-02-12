#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>

/**
 * executes the sum of the elements' values
 * @param elements list of values
 * @param num length of elements list
 * @return total sum 
 */
float sum(const float *elements, const uint32_t num);

/**
 * executes the average of the elements' values
 * @param elements list of values
 * @param num length of elements list
 * @return total average
 */
float avg(const float *elements, const uint32_t num);

/**
 * to use with the conways stencil to simulate Conway's game of life
 * @param list of values
 * @param num length of elements list
 * @return the new cell value
 */
float conways_gol(const float *elements, const uint32_t num);

#endif
