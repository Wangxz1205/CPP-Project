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
            cout << "   ";
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
    m_roi = true;
}
