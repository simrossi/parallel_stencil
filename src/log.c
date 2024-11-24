#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include "common.h"

static FILE *log_file = NULL;

void log_init() {
  log_file = fopen(LOG_FILE, "w");

  if (log_file == NULL) {
    fprintf(stderr, "Error opening specified log file!\n");
    exit(1);
  }
}

void log_write(const char *format, ...) {
  if (log_file == NULL) {
    log_init();
  }

  va_list args;
  va_start(args, format);

  // Print formatted log entry to the log file
  vfprintf(log_file, format, args);
  va_end(args);

  fflush(log_file);
}

void log_cleanup() {
  if (log_file != NULL) {
    fclose(log_file);
    log_file = NULL;
  }
}
