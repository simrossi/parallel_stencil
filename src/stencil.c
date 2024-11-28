#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "types.h"

float euclidian_distance(const uint32_t* point1, const uint32_t* point2, const uint32_t dimensions);
float manhattan_distance(const uint32_t* point1, const uint32_t* point2, const uint32_t dimensions);
float calculate_weight(const float distance, const uint32_t dimensions, const uint32_t range);
uint32_t moore(const Matrix matrix, const uint32_t* indices, const uint32_t range, Neighbor* neighbors);
uint32_t von_neumann(const Matrix matrix, const uint32_t* indices, const uint32_t range, Neighbor* neighbors);

// Compute euclidian distance given the indices of two elements
float euclidian_distance(const uint32_t* point1, const uint32_t* point2, const uint32_t dimensions) {
  float sum = 0.0;

  for (uint32_t i = 0; i < dimensions; i++) {
    int32_t diff = (int32_t)(point1[i] - point2[i]);
    sum += (float)(diff * diff);
  }

  return sqrt((float)sum);
}

// Compute manhattan distance given the indices of two elements
float manhattan_distance(const uint32_t* point1, const uint32_t* point2, const uint32_t dimensions) {
  float sum = 0.0;

  for (uint32_t i = 0; i < dimensions; i++) {
    int32_t diff = abs((int32_t)(point1[i] - point2[i]));
    sum += diff;
  }

  return sum;
}

// Compute weight (linear decay | Gaussian decay | Inverse distance)
float calculate_weight(const float distance, const uint32_t dimensions, const uint32_t range) {
  // Linear decay
  const float max_distance = range * sqrt(dimensions);
  return 1.0f - (distance / max_distance);

  // Gaussian decay
  /*float sigma = range / 2.0f;
  return exp(-(distance * distance) / (2 * sigma * sigma));*/

  // Inverse distance
  /*if (distance < 0.0001f) return 1.0f;  // Avoid division by zero for center cell
  float max_distance = range * sqrt(dimensions);
  return 1.0f / (1.0f + (distance / max_distance));*/
}

// Compute the Moore neighborhood
uint32_t moore(const Matrix matrix, const uint32_t* indices, const uint32_t range, Neighbor* neighbors) {
  uint32_t size = 1;
  uint32_t starts[MAX_DIMS], ends[MAX_DIMS], lengths[MAX_DIMS], curr[MAX_DIMS];

  if (range == 0) {
    fprintf(stderr, "Invalid range specified: %d\n", range);
    exit(1);
  }

  // Compute the regions affected by the stencil for every dimension
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t index = indices[i];

    starts[i] = (index > range ? index - range : 0);
    ends[i] = (index + range < matrix.sizes[i] ? index + range : matrix.sizes[i] - 1);
    lengths[i] = (ends[i] - starts[i]) + 1;
    size *= lengths[i];
  }

  // Assign the start of the region to the current position
  memcpy(curr, starts, sizeof(starts));

  // For every element of the region store its value and calculate its weight
  for (uint32_t i = 0; i < size; i++) {
    Neighbor neigh;
    float distance = euclidian_distance(indices, curr, matrix.dimensions);
    float weight = calculate_weight(distance, matrix.dimensions, range);

    neigh.value = matrix.data[indices_to_offset(matrix, curr)];
    neigh.weight = weight;
    neighbors[i] = neigh;

    // Update current position
    curr[matrix.dimensions - 1]++;
    for (uint32_t j = matrix.dimensions - 1; i != size - 1 && j > 0; j--) {
      if (curr[j] >= starts[j] + lengths[j]) {
        curr[j] = starts[j];
        curr[j - 1]++;
      }
    }
  }

  return size;
}

// Compute the von Neumann neighborhood
uint32_t von_neumann(const Matrix matrix, const uint32_t* indices, const uint32_t range, Neighbor* neighbors) {
  uint32_t size = 0;
  uint32_t starts[MAX_DIMS], ends[MAX_DIMS], curr[MAX_DIMS];

  if (range == 0) {
    fprintf(stderr, "Invalid range specified: %d\n", range);
    exit(1);
  }

  //compute the regions affected by the stencil for every dimension
  for (uint32_t i = 0; i < matrix.dimensions; i++) {
    uint32_t index = indices[i];
    starts[i] = (index > range ? index - range : 0);
    ends[i] = (index + range < matrix.sizes[i] ? index + range : matrix.sizes[i] - 1);
  }

  memcpy(curr, starts, sizeof(starts));

  //iterate through all the positions in the defined region
  for (;;) {
    uint32_t dist = manhattan_distance(indices, curr, matrix.dimensions);

    //if in the range, store the neighbor
    if (dist <= range) {
      Neighbor neigh;
      float weight = calculate_weight(dist, matrix.dimensions, range);

      neigh.value = matrix.data[indices_to_offset(matrix, curr)];
      neigh.weight = weight;
      neighbors[size++] = neigh;
    }

    // Update current position to explore the next neighbor
    int done = 1;
    for (uint32_t i = matrix.dimensions; i-- > 0;) {
      curr[i]++;
      if (curr[i] <= ends[i]) {
        done = 0;
        break;
      }
      curr[i] = starts[i];
    }
    if (done) break;
  }

  return size;
}