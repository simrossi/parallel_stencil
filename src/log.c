#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"

static FILE *log_file = NULL;

void log_init(const char *filename)
{
    if (filename == NULL)
    {
        return;
    }

    log_file = fopen(filename, "w");

    if (log_file == NULL)
    {
        fprintf(stderr, "Error: Failed to open specified log file.\n");
        exit(1);
    }
}

void log_write(const char *format, ...)
{
    if (log_file == NULL)
    {
        return;
    }

    va_list args;
    va_start(args, format);

    // Print formatted log entry to the log file
    vfprintf(log_file, format, args);
    va_end(args);

    fflush(log_file);
}

void log_cleanup()
{
    if (log_file != NULL)
    {
        fclose(log_file);
        log_file = NULL;
    }
}
