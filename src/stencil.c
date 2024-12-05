#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "matrix.h"
#include "stencil.h"
#include "types.h"
#include "utils.h"

static Stencil stencil;
static Operation operation;
static Matrix stencil_matrix; // Matrix representation of the stencil

void init_stencil(const Stencil _stencil, const Operation _operation)
{
    stencil = _stencil;
    operation = _operation;
    stencil_matrix = stencil_to_matrix(stencil);
}

void stencil_cleanup()
{
    free(stencil.data);
    free(stencil_matrix.data);
}

float compute_stencil(const Matrix matrix, const uint32_t offset)
{
    if (matrix.dimensions != stencil.dimensions)
    {
        fprintf(stderr, "Error: The dimensions of the provided matrix and stencil are not the same.\n");
        exit(1);
    }

    uint32_t region_size = 1, actual_size = 0;
    int32_t starts[MAX_DIMS], ends[MAX_DIMS], lengths[MAX_DIMS], curr[MAX_DIMS];

    // Compute indices of current element
    uint32_t indices[MAX_DIMS];
    offset_to_indices(matrix, offset, indices);

    // Compute the regions affected by the stencil for every dimension
    for (uint32_t i = 0; i < matrix.dimensions; i++)
    {
        uint32_t index = indices[i];
        uint32_t space_before = stencil.center[i];
        uint32_t space_after = stencil.sizes[i] - (stencil.center[i] + 1);

        starts[i] = index - space_before;//(index > space_before ? index - space_before : 0);
        ends[i] = index + space_after;//(index + space_after < matrix.sizes[i] ? index + space_after : matrix.sizes[i] - 1);
        lengths[i] = (ends[i] - starts[i]) + 1;
        region_size *= lengths[i];
    }

    // Assign the start of the region to the current position
    memcpy(curr, starts, sizeof(starts));

    // For every element of the region store its value and calculate its weight
    float neighbors[region_size];
    for (uint32_t i = 0; i < region_size; i++)
    {
        int32_t offsets[MAX_DIMS];
        uint32_t stencil_indices[MAX_DIMS];

        // Map the current element to the stencil pattern
        calculate_offsets(matrix, indices, (uint32_t*)curr, offsets);
        for (uint32_t j = 0; j < matrix.dimensions; j++)
        {
            stencil_indices[j] = stencil.center[j] + offsets[j];
        }

        // Retrieve the stencil value corresponding to the current element
        float stencil_value = stencil.data[indices_to_offset(stencil_matrix, stencil_indices)];

        // Consider only stencil values different than zero
        if (stencil_value != 0)
        {
            bool outside = is_outside(matrix, curr);

            if (outside) {
                uint32_t closest_indices[MAX_DIMS];
                closest_point(matrix, curr, closest_indices);

                // Set the value to zero if the element affects the stencil while also being outside of the matrix
                neighbors[actual_size++] = matrix.data[indices_to_offset(matrix, closest_indices)] * stencil_value;
            } else {
                // Store the neighbors and scale them by the stencil value
                neighbors[actual_size++] = matrix.data[indices_to_offset(matrix, (uint32_t*)curr)] * stencil_value;
            }
        }

        // Update current position
        curr[matrix.dimensions - 1]++;
        for (uint32_t j = matrix.dimensions - 1; i != region_size - 1 && j > 0; j--)
        {
            if (curr[j] >= starts[j] + lengths[j])
            {
                curr[j] = starts[j];
                curr[j - 1]++;
            }
        }
    }

    return operation(neighbors, actual_size); // Compute the final value
}
