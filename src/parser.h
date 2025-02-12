#ifndef PARSER_H
#define PARSER_H

#define MAX_LINE_LENGTH 256

#include "types.h"

/**
 * read input file and initialize Matrix data structure
 * in particular, allocate a contiguos data buffer to store the
 * input's values
 * @param filename location of the input file to be read
 * @return the allocated data structure
 */
Matrix read_file(const char *filename);

/**
 * write the matrix into an output file
 * @param matrix data structure that represents the matrix used in ISL
 * @param filename location of the output file to be written
 */
void write_file(const Matrix matrix, const char *filename);

/**
 * same as read_file but the input file has a binary format
 * @param filename location of the input file to be read
 * @return the allocated data structure
 */
Matrix read_binfile(const char *filename);

/**
 * same as write_file but the output file will have a binary format
 * @param matrix data structure that represents the matrix used in ISL
 * @param filename location of the output file to be written
 */
void write_binfile(const Matrix matrix, const char *filename);

#endif
