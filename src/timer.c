#include <time.h>
#include "timer.h"

#ifndef PARALLEL

// Start the clock
clock_t init_timer()
{
    return clock();
}

// Return the elapsed time in seconds
double get_time(clock_t timer)
{
    float seconds = (float)(clock() - timer) / CLOCKS_PER_SEC;
    return seconds;
}

#else
#endif
