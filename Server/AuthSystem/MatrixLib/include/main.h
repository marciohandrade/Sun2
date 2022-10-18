#ifndef _MAIN_HEAD
#define _MAIN_HEAD

// for size_t on posix platforms
#include <stdlib.h>
#include <stdio.h>

// for memset
#include <string.h>

#define SEED_SIZE   16
typedef unsigned char matrix_seed[SEED_SIZE];

void matrix_gen(const matrix_seed seed, int matrix_size, int matrix[], const int matrix_base, const int matrix_data_size);
void PrintSeed(matrix_seed);
int genseed(char *mc_seed, matrix_seed seed);

#endif
