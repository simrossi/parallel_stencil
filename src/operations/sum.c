#include <stdbool.h>
#include "../types.h"

float sum(const Neighbor* neighbors, const uint32_t num_neighs, const bool weighted);

float sum(const Neighbor* neighbors, const uint32_t num_neighs, const bool weighted) {
  float sum = 0;
  for (uint32_t i = 0; i < num_neighs; i++) {
    sum += neighbors[i].value * (weighted ? neighbors[i].weight : 1.0);
  }

  return sum;
}
