#ifndef PARSER_H
#define PARSER_H

#define LINE_DIM 256 // Define max number of characters in header line

#include "types.h"

Matrix readfile(const char* filename);
void writefile(const char* filename, Matrix matrix);

#endif
