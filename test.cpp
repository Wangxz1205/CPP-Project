#include <iostream>
#include <typeinfo>
#include <cstring>
#include <algorithm>

using namespace std;

template <typename T>
class Matrix
{
private:
    // Number of rows, columns, and channels in the matrix
    size_t m_numRows;
    size_t m_numCols;
    size_t m_numChannels;

    // Pointer to the matrix data
    T *m_data;

    // Flag to indicate whether a ROI is set
    bool m_roi;

    // Starting row and column, and number of rows and columns in the ROI
    size_t m_roiStartRow;
    size_t m_roiStartCol;
    size_t m_roiNumRows;
    size_t m_roiNumCols;

public:
    Matrix(size_t numRows, size_t numCols, size_t numChannels)
        : m_numRows(numRows), m_numCols(numCols), m_numChannels(numChannels)
    {
        // Allocate memory for the matrix data
        m_data = new T[numRows * numCols * numChannels];
    }
    // Destructor to free memory when the matrix is no longer needed
    ~Matrix() { cout << "xxxgggg" << endl;delete[] m_data; }

    // Overload the assignment operator to avoid memory hard copy
    Matrix &operator=(const Matrix &other)
    {
        cout << "xxxxxx" << endl;
        // Check for self-assignment
        if (this == &other)
            return *this;

        // Free the current matrix data
        delete[] m_data;

        // Copy the number of rows, columns, and channels from the other matrix
        m_numRows = other.m_numRows;
        m_numCols = other.m_numCols;
        m_numChannels = other.m_numChannels;

        // Allocate new memory for the matrix data and copy the data from the other matrix
        m_data = new T[m_numRows * m_numCols * m_numChannels];
        memcpy(m_data, other.m_data, m_numRows * m_numCols * m_numChannels * sizeof(T));

        return *this;
    }

    // Overload the equality operator to compare two matrices
    bool operator==(const Matrix &other) const
    {
        // Check if the two matrices have the same number of rows, columns, and channels
        if (m_numRows != other.m_numRows || m_numCols != other.m_numCols || m_numChannels != other.m_numChannels)
            return false;

        // Compare the matrix data element by element
        for (size_t i = 0; i < m_numRows * m_numCols * m_numChannels; i++)
        {
            if (m_data[i] != other.m_data[i])
                return false;
        }

        return true;
    }

    // Overload the addition operator to add two matrices
    Matrix operator+(const Matrix &other) const
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
            result.m_data[i] = m_data[i] + other.m_data[i];
        }
        return result;
    }

    // Overload the subtraction operator to subtract two matrices
    Matrix operator-(const Matrix &other) const
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
            result.m_data[i] = m_data[i] - other.m_data[i];
        return result;
    }

    // Overload the multiplication operator to multiply a matrix by a scalar
    Matrix operator*(const T &scalar) const
    {
        // Create a new matrix to store the result
        Matrix result(m_numRows, m_numCols, m_numChannels);

        // Multiply each element of the matrix by the scalar and store the result in the new matrix
        for (size_t i = 0; i < m_numRows * m_numCols * m_numChannels; i++)
            result.m_data[i] = m_data[i] * scalar;

        return result;
    }

    // Overload the multiplication operator to multiply two matrices
    Matrix operator*(const Matrix &other) const
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
                    result.m_data[i * other.m_numCols * m_numChannels + j * m_numChannels + k] = 0;
                    for (size_t l = 0; l < m_numCols; l++)
                        result.m_data[i * other.m_numCols * m_numChannels + j * m_numChannels + k] +=
                            m_data[i * m_numCols * m_numChannels + l * m_numChannels + k] * other.m_data[l * other.m_numCols * other.m_numChannels + j * other.m_numChannels + k];
                }
            }
        }

        return result;
    }

    void printmatrix()
    {
        for (size_t i = 0; i < m_numRows; i++)
        {
            for (size_t j = 0; j < m_numCols; j++)
            {
                for (size_t k = 0; k < m_numChannels; k++)
                {
                    cout << m_data[i * m_numCols * m_numChannels + j * m_numChannels + k] << " ";
                }
                cout << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    // Get the number of rows in the matrix
    size_t numRows() const { return m_numRows; }

    // Get the number of columns in the matrix
    size_t numCols() const { return m_numCols; }

    // Get the number of channels in the matrix
    size_t numChannels() const { return m_numChannels; }

    // Get a pointer to the matrix data
    T *data() { return m_data; }

    // Get a const pointer to the matrix data (for read-only access)
    const T *data() const { return m_data; }

    // Set the region of interest (ROI) in the matrix
    void setROI(size_t startRow, size_t startCol, size_t numRows, size_t numCols)
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
};

int main()
{
    // Create two 3x3x3 float matrices
    Matrix<float> matrix1(3, 3, 3);
    Matrix<float> matrix2(3, 3, 3);

    // Set the elements of the first matrix
    for (size_t i = 0; i < matrix1.numRows(); i++)
    {
        for (size_t j = 0; j < matrix1.numCols(); j++)
        {
            for (size_t k = 0; k < matrix1.numChannels(); k++)
            {
                matrix1.data()[i * matrix1.numCols() * matrix1.numChannels() + j * matrix1.numChannels() + k] = i + j + k;
            }
        }
    }

    // Set the elements of the second matrix
    for (size_t i = 0; i < matrix2.numRows(); i++)
    {
        for (size_t j = 0; j < matrix2.numCols(); j++)
        {
            for (size_t k = 0; k < matrix2.numChannels(); k++)
            {
                matrix2.data()[i * matrix2.numCols() * matrix2.numChannels() + j * matrix2.numChannels() + k] = i * j * k;
            }
        }
    }

    // Print the first matrix
    cout << "Matrix 1:" << endl;
    matrix1.printmatrix();

    // Print the second matrix
    std::cout << "Matrix 2:" << std::endl;
    matrix2.printmatrix();

    // Assign matrix1 to matrix2
    // matrix2 = matrix1;

    // Compare matrix1 and matrix2
    if (matrix1 == matrix2)
        cout << "Matrix 1 and matrix 2 are equal" << endl;
    else
        cout << "Matrix 1 and matrix 2 are not equal" << endl;
    cout << endl;

    // Add matrix1 and matrix2
    Matrix<float> matrix3 = matrix1 + matrix2;
    // Print the result of the addition
    cout << "Matrix 1 + matrix 2:" << endl;
    cout << "xyt" << endl;
    matrix3.printmatrix();

    // Subtract matrix2 from matrix1
    Matrix<float> matrix4 = matrix1 - matrix2;

    // Print the result of the subtraction
    cout << "Matrix 1 - matrix 2:" << endl;
    matrix4.printmatrix();

    // Multiply matrix1 by a scalar
    Matrix<float> matrix5 = matrix1 * 2;

    // Print the result of the multiplication
    std::cout << "Matrix 1 * 2:" << std::endl;
    matrix5.printmatrix();

    // Multiply matrix1 by matrix2
    matrix3 = matrix1 * matrix2;

    // Print the result of the multiplication
    std::cout << "Matrix 1 * matrix 2:" << std::endl;
    matrix3.printmatrix();

    // Set the ROI of matrix1 to the top left 2x2 subregion
    matrix1.setROI(0, 0, 2, 2);

    // Print the ROI parameters
    std::cout << "ROI start row: " << matrix1.roiStartRow() << std::endl;
    std::cout << "ROI start column: " << matrix1.roiStartCol() << std::endl;
    std::cout << "ROI number of rows: " << matrix1.roiNumRows() << std::endl;
    std::cout << "ROI number of columns: " << matrix1.roiNumCols() << std::endl;

    return 0;
}