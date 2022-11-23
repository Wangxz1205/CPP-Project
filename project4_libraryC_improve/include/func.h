#ifndef __FUNC_H__
#define __FUNC_H__

#include "malloc.h"
#include "assert.h"
#include "string.h"
#include <time.h>

typedef struct
{
    int row;
    int column;
    float *pdata;
} matrix;




matrix *createMatrix(int row, int column, float *data);

matrix *create_random_matrix(int row, int column);

void deleteMatrix(matrix *m);

matrix *matmul_plain(matrix *m1, matrix *m2);

matrix *matmul_improved(matrix *m1, matrix *m2);

matrix *matmul_improved_omp(matrix *m1, matrix *m2);

void printMatrix(matrix *m);

#endif