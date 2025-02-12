#ifndef LOG_H
#define LOG_H

/**
 * open and cancel past data or create a new log file 
 * @param filename file path of the log file to be written
 */
void log_init(const char *filename);

/**
 * write a formatted log entry to the log file
 * @param format specify print format for a new entry in log file
 * @param ... variable number of entries to be written
 */
void log_write(const char *format, ...);

/**
 * close and clean the log file
 */
void log_cleanup();

#endif
