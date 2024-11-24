#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

void log_init();
void log_write(const char *format, ...);
void log_cleanup();

#endif
