#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include <typeinfo>
#include <cstring>
#include <algorithm>
#include <memory>
#include <fstream>
#include <string>
#include "omp.h"
#include <cblas.h>
#include "malloc.h"
#include <immintrin.h>
#include <cstdlib>
#include <chrono>

using namespace std;

#define TIME_START start = std::chrono::steady_clock::now();
#define TIME_END(NAME)                                                                     \
    end = std::chrono::steady_clock::now();                                                \
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); \
    cout << (NAME) << " duration = " << duration << "ms" << endl;

template <typename T>
class Matrix;

template <typename T>
class Matrix
{
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

public:
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
};

#endif
