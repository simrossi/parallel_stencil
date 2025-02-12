#ifndef TIMER_H
#define TIMER_H

#include <time.h>

/**
 * initialize the clock used for logging
 * @return clock object
 */
clock_t init_timer();

/**
 * return the elapsed time in seconds
 * @return elapsed time in seconds
 */
double get_time();

#endif
