# CS205 C/ C++ Programming - Project4

<font size=3>**Name:** 王习之（Wang Xizhi）

**SID:** 11911818</font>

## Part1 - Analysis
 
---                                                                   
第四次的project我是在第三次project的基础上对矩阵乘法进行加速。因为矩阵乘法的计算需要占用大量的内存，同时数字的乘法可以多线程并行运算，所以运用了intel的SIMD加速方法，同时也用到了OpenMp。同时还有对plain的计算也做了优化的加速。

根据矩阵乘法的定义的基础版: 使用基础的for循环，单独计算结果矩阵的每一个数字，再一个一个地输入。

改进版的矩阵乘法：因为基础版的矩阵乘法提取数据的时候指针是跳跃的，读取时会花费很多时间，所以考虑计算的时候不跳跃指针，同时也能避免转置矩阵。所以在计算结果的时候，是先将A矩阵的第一列乘B矩阵，得到结果矩阵的第一层，最后再层层相加，这样就让指针更加连续，提高了运算速率。

使用SIMD: 主要使用了 __m512 这个类型的数据存储向量，因为我们的测试矩阵最小的为16*16，所以我考虑了用16线程并行计算。

使用OpenMP: 在使用了 SIMD 的代码上加上了 OpenMP 的指令。

使用OpenBLAS: 通过安装 OpenBLAS 的库并调用得到矩阵。

最后测试的时候还在ARM结构上运行一遍，测试了运行时长。

下面是矩阵的结构体：

```c++
typedef struct
{
    size_t row;
    size_t column;
    float *pdata;
} matrix;
```

所有的代码以及报告都可以在我的github中看到：
https://github.com/Wangxz1205/CPP-Project

or

https://github.com/Wangxz1205/CPP-Project.git

## Part2 - Code

---
### 普通矩阵乘法

这里的矩阵乘法我没有使用最基础版本的，而是放的修改指针提取顺序的版本，同时之后的计算结果也都用的改进后的版本。

```c
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
```

### SIMD矩阵乘法
```c
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
```

### OpenMp矩阵乘法
```c
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
```

### OpenBlas矩阵乘法
```c
cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1->row, m2->column, m1->column, 1.0, m1->pdata, m1->column, m2->pdata, m2->column, 0.0, m4->pdata, m4->column);
```

## Part3 - Result & Verification

---
以下分别测试了 $16\times16,\ 128\times128,\ 1k\times1k,\ 8k\times8k$ 时的运行时间，16k 和 64k 因为太大，内存不够所以没有进行测试。
****         | $16\times16$ | $128\times128$ | $1k\times1k$ | $8k\times8k$ 
:------------:|:------------:|:--------------:|:----------------:|:----------------:
 **plain**    |  0.000022s |     0.010612s  |      5.272622s    |      ~45min           
 **SIMD**     | 0.000005s    |    0.000391s   |   0.096695s     |  106.455070s    
 **OpenMP**   |  0.000004s    |   0.001321s    |   0.108397s     |  106.240791s     
 **OpenBLAS** | 0.000014s    |    0.001428s    |  0.079011s      |  2.802694s  
<!-- Test case #1: add/minus/multiple/division

![Alternative text](pic/1.png) -->

## Part4 - Difficulties & Solutions

---
首先的问题就是在每一次使用了或者新建了指针之后，都要判断指针是否为空，要对每一个参数进行判断，让代码格式更准确。


