#include "func.h"

matrix *createMatrix(int row, int column, float *data)
{
    matrix *m = (matrix *)malloc(sizeof(matrix));
    m->row = row;
    m->column = column;
    m->pdata = (float *)malloc(row * column * sizeof(float));

    for (int i = 0; i < row * column; i++)
    {
        *m->pdata = *data;
        m->pdata++;
        data++;
    }
    m->pdata -= row * column;
    return m;
}

void deleteMatrix(matrix *m)
{
    if (m == NULL)
        return;
    free(m->pdata);
    free(m);
    m = NULL;
}

matrix *copyMatrix(matrix *m)
{
    float *newdata = (float *)malloc(m->row * m->column * sizeof(float));
    memcpy(newdata, m->pdata, m->row * m->column * sizeof(float));
    matrix *mcopy = createMatrix(m->row, m->column, newdata);
    free(newdata);
    return mcopy;
}

matrix *addMatrix(matrix *m1, matrix *m2)
{
    assert(m1->row == m2->row && m1->column == m2->column);

    float *newdata1 = m1->pdata;
    float *newdata2 = m2->pdata;
    float *newdata3 = (float *)malloc(m1->row * m1->column * sizeof(float));
    for (int i = 0; i < m1->row * m1->column; ++i)
    {
        *newdata3 = *newdata1 + *newdata2;
        newdata1++, newdata2++, newdata3++;
    }
    newdata3 -= m1->row * m1->column;
    matrix *madd = createMatrix(m1->row, m1->column, newdata3);
    free(newdata3);
    return madd;
}

matrix *subtractMatrix(matrix *m1, matrix *m2)
{
    assert(m1->row == m2->row && m1->column == m2->column);

    float *newdata1 = m1->pdata;
    float *newdata2 = m2->pdata;
    float *newdata3 = (float *)malloc(m1->row * m1->column * sizeof(float));
    for (int i = 0; i < m1->row * m1->column; ++i)
    {
        *newdata3 = *newdata1 - *newdata2;
        newdata1++, newdata2++, newdata3++;
    }
    newdata3 -= m1->row * m1->column;
    matrix *msub = createMatrix(m1->row, m1->column, newdata3);
    free(newdata3);
    return msub;
}

matrix *multiplyMatrix(matrix *m1, matrix *m2)
{
    assert(m1->column == m2->row);

    float *newdata = (float *)malloc(m1->row * m2->column * sizeof(float));
    for (int i = 0; i < m1->row * m2->column; i++)
    {
        float sum = 0;
        for (int j = 0; j < m1->column; j++)
        {
            sum += *(m1->pdata + i / m2->column * m1->column + j) * *(m2->pdata + i % m2->column + j * m2->column);
        }
        *newdata = sum;
        newdata++;
    }
    newdata -= m1->row * m2->column;
    matrix *mmul = createMatrix(m1->row, m2->column, newdata);
    free(newdata);
    return mmul;
}

matrix *addScalar(matrix *m, float scalar)
{
    float *newdata = (float *)malloc(m->row * m->column * sizeof(float));
    for (int i = 0; i < m->row * m->column; i++)
    {
        *newdata = *m->pdata + scalar;
        m->pdata++, newdata++;
    }
    m->pdata -= m->row * m->column;
    newdata -= m->row * m->column;
    matrix *sadd = createMatrix(m->row, m->column, newdata);
    free(newdata);
    return sadd;
}

matrix *subtractScalar(matrix *m, float scalar)
{
    float *newdata = (float *)malloc(m->row * m->column * sizeof(float));
    for (int i = 0; i < m->row * m->column; i++)
    {
        *newdata = *m->pdata - scalar;
        m->pdata++, newdata++;
    }
    m->pdata -= m->row * m->column;
    newdata -= m->row * m->column;
    matrix *ssub = createMatrix(m->row, m->column, newdata);
    free(newdata);
    return ssub;
}

matrix *multiplyScalar(matrix *m, float scalar)
{
    float *newdata = (float *)malloc(m->row * m->column * sizeof(float));
    for (int i = 0; i < m->row * m->column; i++)
    {
        *newdata = *m->pdata * scalar;
        m->pdata++, newdata++;
    }
    m->pdata -= m->row * m->column;
    newdata -= m->row * m->column;
    matrix *smul = createMatrix(m->row, m->column, newdata);
    free(newdata);
    return smul;
}

float Max(matrix *m)
{
    float max = __FLT_MIN__;
    for (int i = 0; i < m->row * m->column; i++)
    {
        max = (*m->pdata >= max) ? *m->pdata : max;
        m->pdata++;
    }
    m->pdata -= m->row * m->column;
    return max;
}

float Min(matrix *m)
{
    float min = __FLT_MAX__;
    for (int i = 0; i < m->row * m->column; i++)
    {
        min = (*m->pdata <= min) ? *m->pdata : min;
        m->pdata++;
    }
    m->pdata -= m->row * m->column;
    return min;
}

void printMatrix(matrix *m)
{
    printf("Matrix: \n");
    for (int i = 0; i < m->row * m->column; i++)
    {
        printf("%f", *m->pdata);
        m->pdata++;
        if ((i + 1) % m->column == 0)
        {
            printf("\n");
        }
        else
        {
            printf(" ");
        }
    }
    m->pdata -= m->row * m->column;
    printf("\n");
}
