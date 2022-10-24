#ifndef __FUNC_H__
#define __FUNC_H__

#include "malloc.h"
#include "assert.h"
#include "string.h"

typedef struct
{
    int row;
    int column;
    float *pdata;
} matrix;

matrix *createMatrix(int row, int column, float *data);

void deleteMatrix(matrix *m);

matrix *copyMatrix(matrix *m);

matrix *addMatrix(matrix *m1, matrix *m2);

matrix *subtractMatrix(matrix *m1, matrix *m2);

matrix *multiplyMatrix(matrix *m1, matrix *m2);

matrix *addScalar(matrix *m, float scalar);

matrix *subtractScalar(matrix *m, float scalar);

matrix *multiplyScalar(matrix *m, float scalar);

float Max(matrix *m);

float Min(matrix *m);

void printMatrix(matrix *m);

#endif