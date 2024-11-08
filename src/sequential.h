#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#include "types.h"

void compute(Matrix matrix, void (*stencil)(Matrix, uint32_t), uint32_t steps);

#endif
