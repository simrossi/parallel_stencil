#ifndef STENCIL_H
#define STENCIL_H

#include "types.h"

// Return the Moore neighborhood of the specified cell
void moore(Matrix matrix, uint32_t* indices, uint32_t range, Cell* cells, uint32_t* num_cell);

// Return the von Neumann neighborhood of the specified cell
void vonNeumann(Matrix matrix, uint32_t* indices, uint32_t range, Cell* cells, uint32_t* num_cell);

#endif
