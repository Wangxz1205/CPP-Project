#include "include/main.h""

int main() {
    float a[3][4] = {{1, 2, 3, 4},
                     {2, 3, 4, 5},
                     {3, 4, 5, 6}};
    matrix *m1 = createMatrix((float *) a, 3, 4);
    float b[4][2] = {{1, 1},
                     {1, 1},
                     {2, 3},
                     {5, 6}};
    matrix *m2 = createMatrix((float *) b, 4, 2);
    printMatrix(multiplyMatrix(m1, m2));
    return 0;
}