#include "func.h"

int main()
{
    float a[3][4] = {{1.665, 2.33, 3.555, 4.2},
                     {2, 3, 4.2, 5},
                     {3.3, 4, 5, 6}};
    float b[4][2] = {{1, 1},
                     {-1, 0},
                     {2, 3},
                     {3.5, 6}};
    // matrix *m1 = createMatrix(3, 4, (float *)a);
    // matrix *m2 = createMatrix(4, 2, (float *)b);
    matrix *m1 = create_random_matrix(16, 16);
    matrix *m2 = create_random_matrix(16, 16);

    printMatrix(m1);
    printMatrix(m2);
    matrix *m5 = matmul_plain(m1, m2);

    printf("Result normal ");
    printMatrix(m5);

    matrix *m6 = matmul_improved(m1, m2);

    printf("Result improved(avx2) ");
    printMatrix(m6);

    return 0;
}