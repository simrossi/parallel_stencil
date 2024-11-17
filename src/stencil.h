#ifndef STENCIL_H
#define STENCIL_H

#include "types.h"

uint32_t moore(const Matrix matrix, const uint32_t* indices, const uint32_t range, Neighbor* neighbors);
uint32_t von_neumann(const Matrix matrix, const uint32_t* indices, const uint32_t range, Neighbor* neighbors);

#endif
