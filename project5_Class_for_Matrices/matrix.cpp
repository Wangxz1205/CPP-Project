#include "matrix.hpp"

template <typename T>
bool Matrix<T>::operator==(const Matrix &other) const
{
    // Check if the two matrices have the same number of rows, columns, and channels
    if (m_numRows != other.m_numRows || m_numCols != other.m_numCols || m_numChannels != other.m_numChannels)
        return false;

    // Compare the matrix data element by element
    for (size_t i = 0; i < m_numRows * m_numCols * m_numChannels; i++)
    {
        if (m_data.get()[i] != other.m_data.get()[i])
            return false;
    }

    return true;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix &other)
{
    // Check for self-assignment
    if (this == &other)
        return *this;

    // Copy the number of rows, columns, channels and the data from the other matrix
    m_numRows = other.m_numRows;
    m_numCols = other.m_numCols;
    m_numChannels = other.m_numChannels;
    m_data = other.m_data;

    return *this;
}

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

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix &other) const
{
    // Check if the two matrices have the same number of rows, columns, and channels
    if (m_numRows != other.m_numRows || m_numCols != other.m_numCols || m_numChannels != other.m_numChannels)
    {
        std::cerr << "Error: Cannot subtract matrices with different dimensions" << std::endl;
        return Matrix(0, 0, 0);
    }

    // Create a new matrix to store the result
    Matrix result(m_numRows, m_numCols, m_numChannels);

    if (result.m_data == NULL || m_data == NULL|| other.m_data == NULL)
    {
        std::cerr << "Error: Cannot add matrices with invalid data" << std::endl;
        return Matrix(0, 0, 0);
    }
    // Subtract the corresponding elements of the two matrices and store the result in the new matrix
    for (size_t i = 0; i < m_numRows * m_numCols * m_numChannels; i++)
        result.m_data.get()[i] = m_data.get()[i] - other.m_data.get()[i];
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T &scalar) const
{
    // Create a new matrix to store the result
    Matrix result(m_numRows, m_numCols, m_numChannels);

    if (result.m_data == NULL || m_data == NULL)
    {
        std::cerr << "Error: Cannot add matrices with invalid data" << std::endl;
        return Matrix(0, 0, 0);
    }

    // Multiply each element of the matrix by the scalar and store the result in the new matrix
    for (size_t i = 0; i < m_numRows * m_numCols * m_numChannels; i++)
        result.m_data.get()[i] = m_data.get()[i];

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix &other) const
{
    // Check if the number of columns in the first matrix is equal to the number of rows in the second matrix
    if (m_numCols != other.m_numRows)
    {
        std::cerr << "Error: Cannot multiply matrices with incompatible dimensions" << std::endl;
        return Matrix(0, 0, 0);
    }

    // Create a new matrix to store the result
    Matrix result(m_numRows, other.m_numCols, m_numChannels);

    if (result.m_data == NULL || m_data == NULL || other.m_data == NULL)
    {
        std::cerr << "Error: Cannot add matrices with invalid data" << std::endl;
        return Matrix(0, 0, 0);
    }

    // Multiply the two matrices and store the result in the new matrix
    for (size_t i = 0; i < m_numRows; i++)
    {
        for (size_t j = 0; j < other.m_numCols; j++)
        {
            for (size_t k = 0; k < m_numChannels; k++)
            {
                result.m_data.get()[i * other.m_numCols * m_numChannels + j * m_numChannels + k] = 0;
                for (size_t l = 0; l < m_numCols; l++)
                    result.m_data.get()[i * other.m_numCols * m_numChannels + j * m_numChannels + k] +=
                        m_data.get()[i * m_numCols * m_numChannels + l * m_numChannels + k] * other.m_data.get()[l * other.m_numCols * other.m_numChannels + j * other.m_numChannels + k];
            }
        }
    }

    return result;
}

template <typename T>
void Matrix<T>::printmatrix()
{
    for (size_t i = 0; i < m_numRows; i++)
    {
        for (size_t j = 0; j < m_numCols; j++)
        {
            for (size_t k = 0; k < m_numChannels; k++)
            {
                cout << m_data.get()[i * m_numCols * m_numChannels + j * m_numChannels + k] << " ";
            }
            cout << "  |  ";
        }
        cout << endl;
    }
    cout << endl;
}

template <typename T>
void Matrix<T>::setROI(size_t startRow, size_t startCol, size_t numRows, size_t numCols)
{
    // Check if the ROI is valid (i.e., it lies within the dimensions of the matrix)
    if (startRow + numRows > m_numRows || startCol + numCols > m_numCols)
    {
        std::cerr << "Error: Invalid ROI" << std::endl;
        return;
    }
    // Set the ROI parameters
    m_roiStartRow = startRow;
    m_roiStartCol = startCol;
    m_roiNumRows = numRows;
    m_roiNumCols = numCols;
    m_numRows = numRows;
    m_numCols = numCols;
    m_roi = true;
    shared_ptr<T> newdata = shared_ptr<T>(new T[numRows * numCols * m_numChannels]);

    if (newdata == NULL)
    {
        std::cerr << "Error: Invalid ROI" << std::endl;
        return;
    }
    
    ;
    for (size_t i = 0; i < numCols * numRows * m_numChannels; i++)
    {
        *(newdata.get() + i) = *(m_data.get() + startRow * m_numCols * m_numChannels + startCol * m_numChannels + i % (numCols * m_numChannels) + i / (numCols * m_numChannels) * (m_numCols * m_numChannels));
    }
    m_data = newdata;
}

template <typename T>
void Matrix<T>::create_random_matrix()
{
    for (size_t i = 0; i < m_numRows * m_numCols * m_numChannels; i++)
        m_data.get()[i] = rand() / (float)(RAND_MAX / 100);
}

template <typename T>
Matrix<T> Matrix<T>::matmul_improved(const Matrix &other)
{
    // Create a new matrix to store the result
    Matrix result(this->m_numRows, other.m_numCols, m_numChannels);

    T *m2new = (T *)aligned_alloc(512, other.m_numRows * other.m_numCols * other.m_numChannels * sizeof(T));

    T *m1new = (T *)aligned_alloc(512, m_numRows * m_numCols * m_numChannels * sizeof(T));

    if (m2new == NULL || m1new == NULL || result.m_data ==NULL)
    {
        cerr << "Failed to allocate memory for a data" << endl;
        return Matrix(0, 0, 0);
    }

    for (size_t k = 0; k < other.m_numChannels; k++)
        for (size_t i = 0; i < other.m_numRows * other.m_numCols; i++)
            *(m2new + i + k * other.m_numRows * other.m_numCols) = *(other.m_data.get() + (i / other.m_numRows) * other.m_numChannels + (i % other.m_numRows) * other.m_numCols * other.m_numChannels + k);

    for (size_t k = 0; k < m_numChannels; k++)
        for (size_t i = 0; i < m_numRows * m_numCols; i++)
            *(m1new + i + k * m_numRows * m_numCols) = *(m_data.get() + i * m_numChannels + k);

    T *newdata = (T *)aligned_alloc(512, 16 * sizeof(T));

    if (newdata == NULL)
    {
        cerr << "Failed to allocate memory for a data." << endl;
        return Matrix(0, 0, 0);
    }

    __m512 a, b, c;

    // #pragma omp parallel for
    for (size_t l = 0; l < m_numChannels; l++)
    {
        for (size_t i = 0; i < m_numRows * other.m_numCols; i++)
        {
            c = _mm512_setzero_ps();
            for (size_t j = 0; j < m_numCols; j += 16)
            {
                a = _mm512_load_ps(m1new + (i / m_numCols) * m_numCols + j + l * m_numRows * m_numCols);
                b = _mm512_load_ps(m2new + (i % other.m_numCols) * other.m_numRows + j + l * other.m_numRows * other.m_numCols);
                c = _mm512_add_ps(c, _mm512_mul_ps(a, b));
                _mm512_store_ps(newdata, c);
            }
            for (size_t k = 0; k < 16; k++)
                *(result.m_data.get() + i * m_numChannels + l) += *(newdata + k);
        }
    }

    free(newdata);
    free(m2new);
    free(m1new);
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::matmul_improved_omp(const Matrix &other)
{
    // Create a new matrix to store the result
    Matrix result(this->m_numRows, other.m_numCols, m_numChannels);

    T *m2new = (T *)aligned_alloc(512, other.m_numRows * other.m_numCols * other.m_numChannels * sizeof(T));

    T *m1new = (T *)aligned_alloc(512, m_numRows * m_numCols * m_numChannels * sizeof(T));

    if (m2new == NULL || m1new == NULL || result.m_data == NULL)
    {
        cerr << "Failed to allocate memory for a data" << endl;
        return Matrix(0, 0, 0);
    }

    for (size_t k = 0; k < other.m_numChannels; k++)
        for (size_t i = 0; i < other.m_numRows * other.m_numCols; i++)
            *(m2new + i + k * other.m_numRows * other.m_numCols) = *(other.m_data.get() + (i / other.m_numRows) * other.m_numChannels + (i % other.m_numRows) * other.m_numCols * other.m_numChannels + k);

    for (size_t k = 0; k < m_numChannels; k++)
        for (size_t i = 0; i < m_numRows * m_numCols; i++)
            *(m1new + i + k * m_numRows * m_numCols) = *(m_data.get() + i * m_numChannels + k);

    T *newdata = (T *)aligned_alloc(512, 16 * sizeof(T));

    if (newdata == NULL)
    {
        cerr << "Failed to allocate memory for a data." << endl;
        return Matrix(0, 0, 0);
    }

    __m512 a, b, c;

#pragma omp parallel for
    for (size_t l = 0; l < m_numChannels; l++)
    {
        for (size_t i = 0; i < m_numRows * other.m_numCols; i++)
        {
            c = _mm512_setzero_ps();
            for (size_t j = 0; j < m_numCols; j += 16)
            {
                a = _mm512_load_ps(m1new + (i / m_numCols) * m_numCols + j + l * m_numRows * m_numCols);
                b = _mm512_load_ps(m2new + (i % other.m_numCols) * other.m_numRows + j + l * other.m_numRows * other.m_numCols);
                c = _mm512_add_ps(c, _mm512_mul_ps(a, b));
                _mm512_store_ps(newdata, c);
            }
            for (size_t k = 0; k < 16; k++)
                *(result.m_data.get() + i * m_numChannels + l) += *(newdata + k);
        }
    }

    free(newdata);
    free(m2new);
    free(m1new);
    return result;
}