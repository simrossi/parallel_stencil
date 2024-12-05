#ifndef PATTERNS_H
#define PATTERNS_H

#include <stdint.h>
#include "types.h"

// Stencil example();
Stencil square_2d(const uint32_t range);
Stencil cross_2d(const uint32_t range);
Stencil vertical_edges_3x3();

#endif
