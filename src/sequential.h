#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#include "types.h"

Matrix compute(Matrix matrix, void (*stencil)(Matrix, Matrix, uint32_t, uint32_t), uint32_t steps);

#endif
