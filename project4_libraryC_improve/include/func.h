#ifndef __FUNC_H__
#define __FUNC_H__

#include <stdio.h>
#include <cblas.h>
#include "malloc.h"
#include "assert.h"
#include "string.h"
#include <stdbool.h>
#include <sys/time.h>
#include <immintrin.h>
#include <omp.h>

typedef struct
{
    size_t row;
    size_t column;
    float *pdata;
} matrix;

matrix *createMatrix(size_t row, size_t column, float *data);

matrix *create_random_matrix(size_t row, size_t column);

bool deleteMatrix(matrix *m);

matrix *matmul_plain(matrix *m1, matrix *m2);

matrix *matmul_improved(matrix *m1, matrix *m2);

matrix *matmul_improved_omp(matrix *m1, matrix *m2);

void printMatrix(matrix *m);

#endif