#include "func.h"

#include <immintrin.h>
#include <omp.h>

matrix *createMatrix(int row, int column, float *data)
{
    assert(row > 0 && column > 0 && data != NULL);

    matrix *m = (matrix *)malloc(sizeof(matrix));
    m->row = row;
    m->column = column;
    m->pdata = (float *)aligned_alloc(512, row * column * sizeof(float));

    for (size_t i = 0; i < row * column; i++)
    {
        *(m->pdata + i) = *data;
        data++;
    }
    return m;
}

matrix *create_random_matrix(int row, int column)
{
    assert(row > 0 && column > 0);
    matrix *m = (matrix *)malloc(sizeof(matrix));
    m->pdata = (float *)aligned_alloc(512, row * column * sizeof(float)); // allocate memory for the first D of the 2D matrix
    m->row = row;
    m->column = column;
    // srand((unsigned)time(0));
    for (int i = 0; i < row * column; i++)
    {
        *(m->pdata + i) = rand() / (float)(RAND_MAX / 100);
    }
    return m;
}

void deleteMatrix(matrix *m)
{
    free(m->pdata);
    free(m);
    m = NULL;
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

matrix *matmul_plain(matrix *m1, matrix *m2)
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

matrix *matmul_improved(matrix *m1, matrix *m2)
{
    assert(m1->column == m2->row && m1->column % 16 == 0);

    float *m2new = (float *)aligned_alloc(512, m2->row * m2->column * sizeof(float));
    for (size_t i = 0; i < m2->row * m2->column; i++)
    {
        *(m2new + i) = *(m2->pdata + (i / m2->row) + (i % m2->row) * m2->column);
    }

    float *newdata = (float *)(aligned_alloc(512, 16 * sizeof(float)));
    float *newdata1 = (float *)(aligned_alloc(512, m1->row * m2->column * sizeof(float)));
    __m512 a, b;

    for (size_t i = 0; i < m1->row * m2->column; i++)
    {
        __m512 c = _mm512_setzero_ps();
        for (size_t j = 0; j < m1->column; j += 16)
        {
            a = _mm512_load_ps(m1->pdata + (i / m1->column) * m1->column + j);
            b = _mm512_load_ps(m2new + (i % m2->column) * m2->row + j);
            c = _mm512_add_ps(c, _mm512_mul_ps(a, b));
            _mm512_store_ps(newdata, c);
        }
        for (size_t k = 0; k < 16; k++)
        {
            *(newdata1 + i) += *(newdata + k);
        }
    }
    matrix *mmul = createMatrix(m1->row, m2->column, newdata1);
    free(newdata);
    free(newdata1);
    free(m2new);
    return mmul;
}

matrix *matmul_improved_omp(matrix *m1, matrix *m2)
{
    assert(m1->column == m2->row && m1->column % 16 == 0);

    float *m2new = (float *)aligned_alloc(512, m2->row * m2->column * sizeof(float));
    for (size_t i = 0; i < m2->row * m2->column; i++)
    {
        *(m2new + i) = *(m2->pdata + (i / m2->row) + (i % m2->row) * m2->column);
    }

    float *newdata = (float *)(aligned_alloc(512, 16 * sizeof(float)));
    float *newdata1 = (float *)(aligned_alloc(512, m1->row * m2->column * sizeof(float)));
    __m512 a, b;

#pragma omp parallel for
    for (size_t i = 0; i < m1->row * m2->column; i++)
    {
        __m512 c = _mm512_setzero_ps();
        for (size_t j = 0; j < m1->column; j += 16)
        {
            a = _mm512_load_ps(m1->pdata + (i / m1->column) * m1->column + j);
            b = _mm512_load_ps(m2new + (i % m2->column) * m2->row + j);
            c = _mm512_add_ps(c, _mm512_mul_ps(a, b));
            _mm512_store_ps(newdata, c);
        }
        for (size_t k = 0; k < 16; k++)
        {
            *(newdata1 + i) += *(newdata + k);
        }
    }
    matrix *mmul = createMatrix(m1->row, m2->column, newdata1);
    free(newdata);
    free(newdata1);
    free(m2new);
    return mmul;
}
