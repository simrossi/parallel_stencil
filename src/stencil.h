#ifndef STENCIL_H
#define STENCIL_H

#include "types.h"
#include <stdint.h>

void init_stencil(const Stencil stencil, const Operation operation);
void stencil_cleanup();
float compute_stencil(const Matrix matrix, const uint32_t offset);

#endif