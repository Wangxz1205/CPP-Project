# CS205 C/ C++ Programming - Project5

<font size=3>**Name:** 王习之（Wang Xizhi）

**SID:** 11911818</font>

## Part1 - Analysis
 
---                                                                   
第五次的project是构建一个关于矩阵的class类，分别包含了矩阵的具体信息，同时让矩阵的具体数字是多样的，包括不限于int，float类型等。除此之外，还实现了矩阵之间的运算，从操作符的重载到矩阵乘法的加速都有涉及。除此之外为了避免硬拷贝，我将data的类型设置为了`shared_ptr`的智能指针。最后，还完成了ROI的功能。

为了避免硬拷贝，用了共享指针之后，可以避免指针的多次删除。

为了让矩阵的数据更加多元化，我使用了模板类，如`template <typename T>`，这样可以在生成矩阵的时候根据需要来生成不同类型的矩阵。

关于操作符的重载，具体操作和之前一样，但是还是要记得检查指针。

对于ROI的计算同样考虑使用智能指针。

下面是矩阵的结构体：

```c++
private:
    // Number of rows, columns, and channels in the matrix
    size_t m_numRows;
    size_t m_numCols;
    size_t m_numChannels;

    // Pointer to the matrix data
    shared_ptr<T> m_data;

    // Flag to indicate whether a ROI is set
    bool m_roi;

    // Starting row and column, and number of rows and columns in the ROI
    size_t m_roiStartRow;
    size_t m_roiStartCol;
    size_t m_roiNumRows;
    size_t m_roiNumCols;
```
同时因为有通道数，故矩阵的表达式如下：
```c++
Matrix 1:
0 1 2   |  1 2 3   |  2 3 4   |  
1 2 3   |  2 3 4   |  3 4 5   |  
2 3 4   |  3 4 5   |  4 5 6   |  
```
此矩阵为3*3的矩阵，通道数为3，左上角的`0 1 2`代表第一个数字的三个通道的数字，以此类推。

所有的代码以及报告都可以在我的github中看到：
https://github.com/Wangxz1205/CPP-Project

or

https://github.com/Wangxz1205/CPP-Project.git

## Part2 - Code

---
### 构造函数以及析构函数

```c++
 Matrix() : m_data(nullptr), m_numRows(0), m_numCols(0), m_numChannels(1)
    {
        // cout << "Constructor() ..." << endl;
    }

    Matrix(size_t numRows, size_t numCols, size_t numChannels)
        : m_numRows(numRows), m_numCols(numCols), m_numChannels(numChannels)
    {
        // cout << "Constructor(size_t) ..." << endl;
        //  Allocate memory for the matrix data
        m_data = shared_ptr<T>(new T[numRows * numCols * numChannels]);
    }

    Matrix(const Matrix &other) : m_numRows(other.m_numRows), m_numCols(other.m_numCols), m_numChannels(other.m_numChannels)
    {
        // cout << "Copying ..." << endl;
        m_data = other.m_data;
    }

    // Destructor to free memory when the matrix is no longer needed
    ~Matrix()
    {
        // cout << "Destructor is running..." << endl;
        m_data.reset();
    }

```

### 所有的成员函数，包括操作符运算以及ROI
```c++
// Overload the assignment operator to avoid memory hard copy
    Matrix &operator=(const Matrix &other);

    // Overload the equality operator to compare two matrices
    bool operator==(const Matrix &other) const;

    // Overload the addition operator to add two matrices
    Matrix operator+(const Matrix &other) const;

    // Overload the subtraction operator to subtract two matrices
    Matrix operator-(const Matrix &other) const;

    // Overload the multiplication operator to multiply a matrix by a scalar
    Matrix operator*(const T &scalar) const;

    // Overload the multiplication operator to multiply two matrices
    Matrix operator*(const Matrix &other) const;

    // Print the matrix
    void printmatrix();

    // Get the number of rows in the matrix
    size_t numRows() const { return m_numRows; }

    // Get the number of columns in the matrix
    size_t numCols() const { return m_numCols; }

    // Get the number of channels in the matrix
    size_t numChannels() const { return m_numChannels; }

    // Get a pointer to the matrix data
    shared_ptr<T> data() { return m_data; }

    // Get a const pointer to the matrix data (for read-only access)
    const shared_ptr<T> data() const { return m_data; }

    // Set the region of interest (ROI) in the matrix
    void setROI(size_t startRow, size_t startCol, size_t numRows, size_t numCols);
    // Reset the ROI to the entire matrix
    void resetROI()
    {
        m_roiStartRow = 0;
        m_roiStartCol = 0;
        m_roiNumRows = m_numRows;
        m_roiNumCols = m_numCols;
        m_roi = false;
    }

    // Check if a ROI is set
    bool isROISet() const { return m_roi; }

    // Get the starting row of the ROI
    size_t roiStartRow() const { return m_roiStartRow; }

    // Get the starting column of the ROI
    size_t roiStartCol() const { return m_roiStartCol; }

    // Get the number of rows in the ROI
    size_t roiNumRows() const { return m_roiNumRows; }

    // Get the number of columns in the ROI
    size_t roiNumCols() const { return m_roiNumCols; }

    void create_random_matrix();

    Matrix matmul_improved(const Matrix &other);

    Matrix matmul_improved_omp(const Matrix &other);
```

### 其中的一种具体定义，以加法操作符为例
```c++
template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix &other) const
{
    // Check if the two matrices have the same number of rows, columns, and channels
    if (m_numRows != other.m_numRows || m_numCols != other.m_numCols || m_numChannels != other.m_numChannels)
    {
        std::cerr << "Error: Cannot add matrices with different dimensions" << std::endl;
        return Matrix(0, 0, 0);
    }

    // Create a new matrix to store the result
    Matrix result(m_numRows, m_numCols, m_numChannels);

    if (result.m_data == NULL || m_data == NULL|| other.m_data == NULL)
    {
        std::cerr << "Error: Cannot add matrices with invalid data" << std::endl;
        return Matrix(0, 0, 0);
    }
    
    // Add the corresponding elements of the two matrices and store the result in the new matrix
    for (size_t i = 0; i < m_numRows * m_numCols * m_numChannels; i++)
    {
        result.m_data.get()[i] = m_data.get()[i] + other.m_data.get()[i];
    }
    return result;
}
```

## Part3 - Result & Verification

---
以下分别书矩阵1和矩阵2的数据：
```c++
Matrix 1:
0 1 2   |  1 2 3   |  2 3 4   |  
1 2 3   |  2 3 4   |  3 4 5   |  
2 3 4   |  3 4 5   |  4 5 6   |  

Matrix 2:
0 0 0   |  0 0 0   |  0 0 0   |  
0 0 0   |  0 1 2   |  0 2 4   |  
0 0 0   |  0 2 4   |  0 4 8   |  
```

两个矩阵相乘结果如下：(加、减等其他操作见github)
```c++
Matrix 1 * matrix 2:
5 14 29   |  8 20 38   |  11 26 47   |  
8 20 38   |  14 29 50   |  20 38 62   |  
11 26 47   |  20 38 62   |  29 50 77   |  
```

ROI设置了取`3*3矩阵`的左上角的`2*2矩阵`，结果如下：
```c++
ROI start row: 0
ROI start column: 0
ROI number of rows: 2
ROI number of columns: 2
0 1 2   |  1 2 3   |  
2 3 4   |  1 2 3   | 
```

最后同样写了利用了AVX和Omp的加速方法的矩阵乘法，也列出了运行时间如下：
```c++
avx duration = 72ms
normal duration = 1427ms
omp duration = 80ms
```


## Part4 - Difficulties & Solutions

---
问题主要出在：

1.智能指针的使用，智能指针本质上不是一个指针，而是一个集合，所以需要用到`.get()`方法去获得真正的地址。

2.在进行复制构造函数时，会默认调用一次构造函数，最后再进行一次析构函数的。


