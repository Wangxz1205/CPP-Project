#include "func.h"

int main()
{
    // float a[3][4] = {{1.665, 2.33, 3.555, 4.2},
    //                  {2, 3, 4.2, 5},
    //                  {3.3, 4, 5, 6}};
    // float b[4][2] = {{1, 1},
    //                  {-1, 0},
    //                  {2, 3},
    //                  {3.5, 6}};
    // matrix *m1 = createMatrix(3, 4, (float *)a);
    // matrix *m2 = createMatrix(4, 2, (float *)b);
    int n = 2*1024;
    matrix *m1 = create_random_matrix(n, n);
    matrix *m2 = create_random_matrix(n, n);

    printf("%d*%d matrices multiplication\n\n", n, n);
    // printMatrix(m1);
    // printMatrix(m2);

    struct timeval start, end;

    // start = clock();

    // matrix *m5 = matmul_plain(m1, m2);

    // end = clock();
    // duration = end - start;
    // printf("%s: duration= %dms\n", "normal", duration);

    // // printf("Result normal ");
    // // printMatrix(m5);

    // start = clock();

    // matrix *m6 = matmul_improved(m1, m2);
    // end = clock();
    // duration = end - start;
    // printf("%s: duration= %dms\n", "SIMD", duration);

    gettimeofday(&start, NULL);

    matrix *m7 = matmul_improved_omp(m1, m2);

    gettimeofday(&end, NULL);
    int timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("omp: %.4f s\n", ((float)timeuse / 1000000.0f));

    deleteMatrix(m7);

    // printf("Result improved(avx2) ");
    // printMatrix(m6);

    return 0;
}