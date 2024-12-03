#include <stdint.h>
#include "operations.h"

float sum(const float* elements, const uint32_t num) {
    float sum = 0;
    for (uint32_t i = 0; i < num; i++) {
        sum += elements[i];
    }

    return sum;
}

float avg(const float* elements, const uint32_t num) {
    return sum(elements, num) / num;
}
