#ifndef PATTERNS_H
#define PATTERNS_H

#include "types.h"
#include <stdint.h>

// Stencil example();
Stencil square_2d(const uint32_t range);
Stencil cross_2d(const uint32_t range);
Stencil vertical_edges_3x3();
Stencil border_movement_detection();
Stencil gabor_kernel();
Stencil conways();

#endif
