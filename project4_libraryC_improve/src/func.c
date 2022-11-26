#include "func.h"

matrix *createMatrix(size_t row, size_t column, float *data)
{
    matrix *m = NULL;

    if (row == 0 || column == 0 || data == NULL)
    {
        fprintf(stderr, "rows and/or colums is 0 or data is empty\n");
        return NULL;
    }

    m = (matrix *)malloc(sizeof(matrix));

    if (m == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a matrix.\n");
        return NULL;
    }

    m->row = row;
    m->column = column;
    m->pdata = (float *)aligned_alloc(512, row * column * sizeof(float));

    if (m->pdata == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the matrix data.\n");
        free(m);
        return NULL;
    }

    for (size_t i = 0; i < row * column; i++)
    {
        *(m->pdata + i) = *data;
        data++;
    }

    return m;
}

matrix *create_random_matrix(size_t row, size_t column)
{
    matrix *m = NULL;

    if (row == 0 || column == 0)
    {
        fprintf(stderr, "rows and/or colums is 0 or data is empty\n");
        return NULL;
    }

    m = (matrix *)malloc(sizeof(matrix));

    if (m == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a matrix.\n");
        return NULL;
    }

    m->pdata = (float *)aligned_alloc(512, row * column * sizeof(float)); // allocate memory for the first D of the 2D matrix
    m->row = row;
    m->column = column;

    if (m->pdata == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the matrix data.\n");
        free(m);
        return NULL;
    }

    for (size_t i = 0; i < row * column; i++)
        *(m->pdata + i) = rand() / (float)(RAND_MAX / 100);

    return m;
}

bool deleteMatrix(matrix *m)
{
    if (!m)
        return false;
    if (m->pdata)
        free(m->pdata);
    free(m);
    return true;
}

void printMatrix(matrix *m)
{
    printf("Matrix: \n");
    for (size_t i = 0; i < m->row * m->column; i++)
    {
        printf("%f", *(m->pdata + i));
        if ((i + 1) % m->column == 0)
            printf("\n");
        else
            printf(" ");
    }
    printf("\n");
}

matrix *matmul_plain(matrix *m1, matrix *m2)
{
    matrix *mmul = NULL;

    if (m1 == NULL || m2 == NULL || (m1->column != m2->row))
    {
        fprintf(stderr, "One or two matrices is empty or data is not matched.\n");
        return NULL;
    }
    mmul = (matrix *)malloc(sizeof(matrix));

    if (mmul == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a matrix.\n");
        return NULL;
    }

    mmul->pdata = (float *)aligned_alloc(512, m1->row * m2->column * sizeof(float)); // allocate memory for the first D of the 2D matrix
    mmul->row = m1->row;
    mmul->column = m2->column;

    if (mmul->pdata == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the matrix data.\n");
        free(mmul);
        return NULL;
    }

    for (size_t i = 0; i < m1->column; i++)
    {
        // float sum = 0;

        for (size_t j = 0; j < mmul->row * mmul->column; j++)
        {
            *(mmul->pdata + j) += *(m1->pdata + (j / m1->column) * m1->column + i) * *(m2->pdata + i * m2->column + j % m2->column);
        }
    }

    return mmul;
}

matrix *matmul_improved(matrix *m1, matrix *m2)
{
    matrix *mmul = NULL;

    if (m1 == NULL || m2 == NULL || (m1->column != m2->row) || m1->column % 16 != 0)
    {
        fprintf(stderr, "One or two matrices is empty or data is not matched or not match the situation of improvement.\n");
        return NULL;
    }
    mmul = (matrix *)malloc(sizeof(matrix));

    if (mmul == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a matrix.\n");
        return NULL;
    }

    float *m2new = (float *)aligned_alloc(512, m2->row * m2->column * sizeof(float));

    if (m2new == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a data.\n");
        return NULL;
    }

    for (size_t i = 0; i < m2->row * m2->column; i++)
        *(m2new + i) = *(m2->pdata + (i / m2->row) + (i % m2->row) * m2->column);

    mmul->pdata = (float *)aligned_alloc(512, m1->row * m2->column * sizeof(float)); // allocate memory for the first D of the 2D matrix
    mmul->row = m1->row;
    mmul->column = m2->column;

    if (mmul->pdata == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the matrix data.\n");
        free(mmul);
        return NULL;
    }

    float *newdata = (float *)(aligned_alloc(512, 16 * sizeof(float)));

    if (newdata == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a data.\n");
        return NULL;
    }

    __m512 a, b;

    // #pragma omp parallel for
    for (size_t i = 0; i < mmul->row * mmul->column; i++)
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
            *(mmul->pdata + i) += *(newdata + k);
    }
    free(newdata);
    free(m2new);
    return mmul;
}

matrix *matmul_improved_omp(matrix *m1, matrix *m2)
{
    matrix *mmul = NULL;

    if (m1 == NULL || m2 == NULL || (m1->column != m2->row) || m1->column % 16 != 0)
    {
        fprintf(stderr, "One or two matrices is empty or data is not matched or not match the situation of improvement.\n");
        return NULL;
    }
    mmul = (matrix *)malloc(sizeof(matrix));

    if (mmul == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a matrix.\n");
        return NULL;
    }

    float *m2new = (float *)aligned_alloc(512, m2->row * m2->column * sizeof(float));

    if (m2new == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a data.\n");
        return NULL;
    }

    for (size_t i = 0; i < m2->row * m2->column; i++)
        *(m2new + i) = *(m2->pdata + (i / m2->row) + (i % m2->row) * m2->column);

    mmul->pdata = (float *)aligned_alloc(512, m1->row * m2->column * sizeof(float)); // allocate memory for the first D of the 2D matrix
    mmul->row = m1->row;
    mmul->column = m2->column;

    if (mmul->pdata == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the matrix data.\n");
        free(mmul);
        return NULL;
    }

    float *newdata = (float *)(aligned_alloc(512, 16 * sizeof(float)));

    if (newdata == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a data.\n");
        return NULL;
    }

    __m512 a, b;

#pragma omp parallel for
    for (size_t i = 0; i < mmul->row * mmul->column; i++)
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
            *(mmul->pdata + i) += *(newdata + k);
    }
    free(newdata);
    free(m2new);
    return mmul;
}