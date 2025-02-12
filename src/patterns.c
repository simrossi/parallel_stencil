#include "patterns.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

/*Stencil example()
{
    const uint32_t tot_size = 3 * 3;
    float *data = malloc(tot_size * sizeof(float));

    float tmp[9] = {
        0.0f, .25f, 0.0f,
        .25f, 1.0f, .25f,
        0.0f, .25f, 0.0f
    };
    memcpy(data, tmp, tot_size * sizeof(float));

    return (Stencil){
        .dimensions = 2,
        .sizes = {3, 3},
        .center = {1, 1},
        .data = data};
}*/

Stencil square_2d(uint32_t range) {
  uint32_t size = range * 2 + 1;
  uint32_t tot_size = size * size;
  float *data = malloc(tot_size * sizeof(float));

  for (uint32_t i = 0; i < tot_size; i++) // Initialize elements to one
  {
    data[i] = 1.0f;
  }

  return (Stencil){.dimensions = 2,
                   .sizes = {size, size},
                   .center = {range, range},
                   .data = data};
}

Stencil cross_2d(uint32_t range) {
  uint32_t size = range * 2 + 1;
  uint32_t tot_size = size * size;
  float *data = malloc(tot_size * sizeof(float));

  for (uint32_t i = 0; i < tot_size; i++) // Initialize elements to zero
  {
    data[i] = 0.0f;
  }

  for (uint32_t i = 0; i < size; i++) // Set vertical middle column
  {
    data[size * i + range] = 1.0f;
  }

  for (uint32_t i = 0; i < size; i++) // Set horizontal middle row
  {
    data[size * range + i] = 1.0f;
  }

  return (Stencil){.dimensions = 2,
                   .sizes = {size, size},
                   .center = {range, range},
                   .data = data};
}

Stencil vertical_edges_3x3() {
  const uint32_t tot_size = 3 * 3;
  float *data = malloc(tot_size * sizeof(float));

  float tmp[9] = {-1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f};
  memcpy(data, tmp, tot_size * sizeof(float));

  return (Stencil){
      .dimensions = 2, .sizes = {3, 3}, .center = {1, 1}, .data = data};
}

Stencil border_movement_detection() {
  const uint32_t tot_size = 3 * 3;
  float *data = malloc(tot_size * sizeof(float));

  float tmp[9] = {-1.0f, -1.0f, -1.0f, -1.0f, 8.0f, -1.0f, -1.0f, -1.0f, -1.0f};
  memcpy(data, tmp, tot_size * sizeof(float));

  return (Stencil){
      .dimensions = 2, .sizes = {3, 3}, .center = {1, 1}, .data = data};
}

Stencil gabor_kernel() {
  const uint32_t tot_size = 3 * 3;
  float *data = malloc(tot_size * sizeof(float));

  float tmp[9] = {-1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
  memcpy(data, tmp, tot_size * sizeof(float));

  return (Stencil){
      .dimensions = 2, .sizes = {3, 3}, .center = {1, 1}, .data = data};
}

Stencil conways() {
  Stencil stencil = square_2d(1);
  stencil.data[4] = 0.5f;

  return stencil;
}
