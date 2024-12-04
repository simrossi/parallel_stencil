#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "common.h"

// Matrix structure
typedef struct
{
    uint32_t total_size;
    uint32_t dimensions;
    uint32_t sizes[MAX_DIMS];
    uint32_t submat_sizes[MAX_DIMS];
    float *data;
} Matrix;

// Stencil structure
typedef struct
{
    uint32_t dimensions;
    uint32_t sizes[MAX_DIMS];
    uint32_t center[MAX_DIMS];
    float *data;
} Stencil;

typedef float (*Operation)(const float *, const uint32_t);

#endif
