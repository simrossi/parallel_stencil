#ifndef AVERAGE_H
#define AVERAGE_H

#include <stdbool.h>
#include "../types.h"

float average(const Neighbor* neighbors, const uint32_t num_neighs, const bool weighted);

#endif
