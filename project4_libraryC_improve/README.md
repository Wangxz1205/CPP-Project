# CS205 C/ C++ Programming - Project3（第四次的还没写）

gcc main.c  -o test -I /opt/OpenBLAS/include/ -L /opt/OpenBLAS/lib/ -lopenblas -lpthread -mavx512f

<font size=3>**Name:** 王习之（Wang Xizhi）

**SID:** 11911818</font>

## Part1 - Analysis

---
第三次的project我认为是在C语言的环境下对于指针和function熟练的掌握。我需要建立一个结构体来储存矩阵的信息，同时不确定矩阵的数量的情况下，需要建立指针类型，同时参数的传递基本都是指针类型（*），指针也就指的是地址，我们平时所用的数组也是一种指针。

```c++
typedef struct
{
    int row;
    int column;
    float *pdata;
} matrix;
```

分析整个题目要求之后，我明白了我们需要做一个类似于包含了多个function的library库，这些function可以实现矩阵之间的加减乘，矩阵与数字的加减乘，矩阵的复制，建立与删除，以及寻找矩阵数中的最大小值等等，所有的方法都是matrix的指针类型，返回的都是指针。

在建立一个新的指针的时候，我们需要用到在c语言中独有的`malloc`方法，去申请一个已知大小的空间给到这个指针。同时，整个的计算过程都是利用指针对应的地址的值去计算的。

有一些function需要先做异常判断，例如矩阵的相乘，需要先判断前一个矩阵的列数与后一个矩阵的行数相等才能做计算，这个时候用到了`assert`方法去进行一个异常的判断处理。

在其他功能我添加了一个矩阵的打印，将矩阵给打印出来，同时也可以更好地对代码进行检查。

所有的代码以及报告都可以在我的github中看到：
https://github.com/Wangxz1205/CPP-Project

or

https://github.com/Wangxz1205/CPP-Project.git

## Part2 - Code

---
这是我的`func.c`文件,main.c的文件比较基础，就不在这里列出，具体内容可以查看github。

```c
#include "func.h"

matrix *createMatrix(int row, int column, float *data)
{
    assert(row > 0 && column > 0 && data != NULL);
    
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

```

## Part3 - Result & Verification

---
以下是我做的一些测试，测试了所有的功能，并且做了输出，验证了输出的正确性。
```
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

    printMatrix(m1);
    printMatrix(m2);
    printMatrix(m3);
    printMatrix(m4_w);
    printMatrix(m5);
    printMatrix(m6);
    printMatrix(m7);
    printMatrix(m8);
    printMatrix(m9);
    printf("%f\n",max);
    printf("%f\n",min);

    return 0;
}
```
最后的答案是：
```
Matrix: 
1.665000 2.000000 300.000000 4.000000
2.000000 3.000000 4.200000 5.000000
3.300000 4.000000 5.000000 6.000000

Matrix: 
1.000000 1.000000
-1.300000 0.000000
2.000000 3.000000
3.500000 6.000000

Matrix: 
1.665000 2.000000 300.000000 4.000000
2.000000 3.000000 4.200000 5.000000
3.300000 4.000000 5.000000 6.000000

Matrix: 
3.330000 4.000000 600.000000 8.000000
4.000000 6.000000 8.400000 10.000000
6.600000 8.000000 10.000000 12.000000

Matrix: 
-1.665000 -2.000000 -300.000000 -4.000000
-2.000000 -3.000000 -4.200000 -5.000000
-3.300000 -4.000000 -5.000000 -6.000000

Matrix: 
613.065002 925.664978
24.000000 44.599998
29.100000 54.299999

Matrix: 
8.165000 8.500000 306.500000 10.500000
8.500000 9.500000 10.700000 11.500000
9.800000 10.500000 11.500000 12.500000

Matrix: 
-8.335000 -8.000000 290.000000 -6.000000
-8.000000 -7.000000 -5.800000 -5.000000
-6.700000 -6.000000 -5.000000 -4.000000

Matrix: 
-4.995000 -6.000000 -900.000000 -12.000000
-6.000000 -9.000000 -12.599999 -15.000000
-9.900000 -12.000000 -15.000000 -18.000000

300.000000
-1.300000
```
<!-- Test case #1: add/minus/multiple/division

![Alternative text](pic/1.png) -->

## Part4 - Difficulties & Solutions

---
首先的问题就是在使用了`malloc`方法之后，应该在不需要用到它的时候对他进行释放，使用`free()`方法将它内存释放。

第二个问题在于当我直接调用`pdata`的指针时，例如在我复制矩阵时，我的思路是将`pdata`进行++处理，去取每一个值，但是在方法最后的时候需要将`pdata`减去`row * column`的值，不要最后返回的矩阵的`pdata`指针的位置是在数组的末尾。而不是最开头。而另一种解决方法就是重新定义一个`float *newdata`去储存原先矩阵的指针，这样就不会改变原先矩阵的值。

在使用`free()`方法的时候，是可以对`NULL`进行处理，返回不做任何操作。所以当出现矩阵为`NULL`的时候，也可以`free`，但是在这之后需要对指针进行`NULL`的赋值操作。
