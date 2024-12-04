#ifndef PARSER_H
#define PARSER_H

#define MAX_LINE_LENGTH 256

#include "types.h"

Matrix read_file(const char *filename);
void write_file(const Matrix matrix, const char *filename);

#endif
