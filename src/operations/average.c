#include <math.h>
#include "sum.h"
#include "../types.h"

float average(const Neighbor* neighbors, const uint32_t num_neighs, const bool weighted);

float average(const Neighbor* neighbors, const uint32_t num_neighs, const bool weighted) {
  return sum(neighbors, num_neighs, weighted) / num_neighs;
}
