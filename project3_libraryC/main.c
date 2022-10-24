#include "func.h"

int main()
{
    float a[3][4] = {{1.665, 2, 300, 4},
                     {2, 3, 4.2, 5},
                     {3.3, 4, 5, 6}};
    float b[4][2] = {{1, 1},
                     {-1.3, 0},
                     {2, 3},
                     {3.5, 6}};
    matrix *m1 = createMatrix(3, 4, (float *)a);
    matrix *m2 = createMatrix(4, 2, (float *)b);

    matrix *m3 = copyMatrix(m1);

    matrix *m4_w = addMatrix(m1, m3);
    matrix *m5 = subtractMatrix(m1, m4_w);
    matrix *m6 = multiplyMatrix(m1, m2);

    matrix *m7 = addScalar(m1, 6.5);
    matrix *m8 = subtractScalar(m1, 10);
    matrix *m9 = multiplyScalar(m1, -3);

    float max = Max(m1);
    float min = Min(m2);

    // deleteMatrix(m1);

    // printMatrix(m1);
    // printMatrix(m2);
    // printMatrix(m3);
    // printMatrix(m4_w);
    // printMatrix(m5);
    // printMatrix(m6);
    // printMatrix(m7);
    // printMatrix(m8);
    // printMatrix(m9);
    // printf("%f\n",max);
    // printf("%f\n",min);

    return 0;
}