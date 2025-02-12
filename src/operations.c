#include "operations.h"
#include <stdbool.h>
#include <stdint.h>

float sum(const float *elements, const uint32_t num) {
  float sum = 0;
  for (uint32_t i = 0; i < num; i++) {
    sum += elements[i];
  }

  return sum;
}

float avg(const float *elements, const uint32_t num) {
  return sum(elements, num) / num;
}

float conways_gol(const float *elements, const uint32_t num) {
  bool alive = false;
  float neigh = sum(elements, num);

  for (uint32_t i = 0; i < num; i++) {
    if (elements[i] > 0.0 && elements[i] < 1.0) {
      alive = true;
      break;
    }
  }

  if (alive) {
    if (neigh >= 2 && neigh < 4) {
      return 1.0f; // Any live cell with two or three live neighbours lives on
                   // to the next generation
    } else {
      // Any live cell with fewer than two live neighbours dies, as if by
      // underpopulation Any live cell with more than three live neighbours
      // dies, as if by overpopulation
      return 0.0f;
    }
  } else if (neigh == 3) {
    return 1.0f; // Any dead cell with exactly three live neighbours becomes a
                 // live cell, as if by reproduction
  } else {
    return 0.0f;
  }
}
