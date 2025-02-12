#ifndef PATTERNS_H
#define PATTERNS_H

#include "types.h"
#include <stdint.h>

// Stencil example();

/**
 * generate a 2D square stencil of size range x range
 * where each cell will have as value 1.0
 * 
 * range = 3
 * 
 * 1 1 1
 * 
 * 1 1 1
 * 
 * 1 1 1
 * 
 * @param range defines the length (number of cells) of one side of the square
 * @return the stencil data structure
 */
Stencil square_2d(const uint32_t range);

/**
 * generate a 2D cross stencil having the orthogonal lines long as range
 * each cell will have as value 1.0 except the cells that don't belong to 
 * the cross and so 0.0
 * 
 * range = 3
 * 
 * 0 1 0
 * 
 * 1 1 1
 * 
 * 0 1 0
 * 
 * @param range defines the length (number of cells) of orthogonal lines
 * @return the stencil data structure
 */
Stencil cross_2d(const uint32_t range);

/**
 * a special 2D stencil used for vertical edges detection
 * 
 * -1 0 1
 * 
 * -2 0 2
 * 
 * -1 0 1
 */
Stencil vertical_edges_3x3();

/**
 * a special 2D stencil used for border movement detection
 * 
 * -1 -1 -1
 * 
 * -1 8 -1
 * 
 * -1 -1 -1
 */
Stencil border_movement_detection();

/**
 * a special 2D stencil used for image processing
 * 
 * -1 -1 0
 * 
 * -1 0 1
 * 
 *  0 1 1
 */
Stencil gabor_kernel();

/**
 * a special 2D stencil used with the conways_gol stencil 
 * to simulate Conway's game of life
 * has the same structure as the square stencil with a different
 * center value
 * 
 * 1 1 1
 * 
 * 1 0.5 1
 * 
 * 1 1 1
 */
Stencil conways();

#endif
