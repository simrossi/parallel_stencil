#include "timer.h"
#include <time.h>

#ifndef PARALLEL

clock_t init_timer() { return clock(); }

double get_time(clock_t timer) {
  float seconds = (float)(clock() - timer) / CLOCKS_PER_SEC;
  return seconds;
}

#else
#endif