#include "src/matrix.cpp"

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
    matrix3.printmatrix();

    // Subtract matrix2 from matrix1
    Matrix<float> matrix4 = matrix1 - matrix2;

    // Print the result of the subtraction
    cout << "Matrix 1 - matrix 2:" << endl;
    matrix4.printmatrix();

    // Multiply matrix1 by a scalar
    Matrix<float> matrix5 = matrix1 * 100;

    // Print the result of the multiplication
    std::cout << "Matrix 1 * 2:" << std::endl;
    matrix5.printmatrix();

    // Multiply matrix1 by matrix2
    Matrix<float> matrix6 = matrix1 * matrix2;

    // Print the result of the multiplication
    std::cout << "Matrix 1 * matrix 2:" << std::endl;
    matrix6.printmatrix();

    // Set the ROI of matrix1 to the top left 2x2 subregion
    matrix1.setROI(0, 0, 2, 2);

    // Print the ROI parameters
    std::cout << "ROI start row: " << matrix1.roiStartRow() << std::endl;
    std::cout << "ROI start column: " << matrix1.roiStartCol() << std::endl;
    std::cout << "ROI number of rows: " << matrix1.roiNumRows() << std::endl;
    std::cout << "ROI number of columns: " << matrix1.roiNumCols() << std::endl;

    return 0;
}