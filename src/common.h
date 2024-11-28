#ifndef COMMON_H
#define COMMON_H

#ifndef MAX_DIMS
#define MAX_DIMS 2 // Maximum number of dimensions
#endif

#ifndef STENCIL_SIZE
#define STENCIL_SIZE 3 // Size of the stencil
/*
The size refers to the number of rows and columns of the matrix that
represents the stencil and so, the neighbor area
*/
#endif

#ifndef ITERATIONS
#define ITERATIONS 1 // Number of iterations to compute
#endif

#ifndef LOG_FILE
#define LOG_FILE "log.txt" // Log file location
#endif

#endif
