#include "func.h"

int timeuse;
struct timeval start, end;
char name;

#define TIME_START gettimeofday(&start, NULL);
#define TIME_END(name)                                                             \
    gettimeofday(&end, NULL);                                                      \
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec; \
    printf("%s: %.6f s\n", name, ((float)timeuse / 1000000.0f));

int main()
{

    int n = 8*1024;
    matrix *m1 = create_random_matrix(n, n);
    matrix *m2 = create_random_matrix(n, n);

    printf("%d*%d matrices multiplication\n\n", n, n);
    // printMatrix(m1);
    // printMatrix(m2);
    TIME_START

    matrix *m4 = create_random_matrix(n, n);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1->row, m2->column, m1->column, 1.0, m1->pdata, m1->column, m2->pdata, m2->column, 0.0, m4->pdata, m4->column);
    TIME_END("OpenBLAS")
    // printMatrix(m4);
    //  deleteMatrix(m4);

    // TIME_START
    // matrix *m5 = matmul_plain(m1, m2);
    // TIME_END("normal")
    // // printMatrix(m5);
    // // deleteMatrix(m5);

    TIME_START
    matrix *m6 = matmul_improved(m1, m2);
    TIME_END("SIMD")
    // printMatrix(m6);
     deleteMatrix(m6);

    TIME_START
    matrix *m7 = matmul_improved_omp(m1, m2);
    TIME_END("omp")
    // printMatrix(m7);
     deleteMatrix(m7);

    return 0;
}