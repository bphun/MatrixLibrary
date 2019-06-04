/**
 * Unit tests for the matrix class. I have only covered a few
 * test cases so suggestions for test cases are welcomed.
 * Thes suggestions can can be submitted to this project's 
 * issue page on Github.
 */

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/Matrix.h"

#define MATRIX_NUM_ROWS 1024
#define MATRIX_NUM_COLS 1024
#define MATRIX_NUM_COLS_HALF MATRIX_NUM_COLS / 2

using namespace std;

TEST(ConstructorTests, defaultConstructor)
{
    Matrix<int> matrix;

    EXPECT_EQ(matrix.cols(), 0);
    EXPECT_EQ(matrix.rows(), 0);
}

TEST(ConstructorTests, rowAndColumnConstructor)
{
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS;

    Matrix<int> matrix(rows, cols);

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(matrix.elementAt(r, c), 0);

    EXPECT_EQ(matrix.rows(), rows);
    EXPECT_EQ(matrix.cols(), cols);
}

TEST(ConstructorTests, specifiedMatrixArray_SquareMatrix)
{
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS;

    int *arr = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++)
        arr[i] = i;

    Matrix<int> matrix(arr, rows, cols);

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(matrix.elementAt(r, c), arr[r * cols + c]);

    EXPECT_EQ(matrix.rows(), rows);
    EXPECT_EQ(matrix.cols(), cols);
}

TEST(ConstructorTests, specifiedMatrixArray_RectangularMatrix)
{
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS_HALF;

    int* arr = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++)
        arr[i] = i;

    Matrix<int> matrix(arr, rows, cols);

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(matrix.elementAt(r, c), arr[r * cols + c]);

    EXPECT_EQ(matrix.rows(), rows);
    EXPECT_EQ(matrix.cols(), cols);
}

TEST(ConstructorTests, specifiedMatrixVector_SquareMatrix)
{
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS;

    vector<int> vec;

    for (int i = 0; i < rows * cols; i++)
        vec.push_back(i);

    Matrix<int> matrix(vec, rows, cols);

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(matrix.elementAt(r, c), vec[r * cols + c]);

    EXPECT_EQ(matrix.rows(), rows);
    EXPECT_EQ(matrix.cols(), cols);
}

TEST(ConstructorTests, specifiedMatrixVector_RectangularMatrix)
{
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS_HALF;

    vector<int> vec;

    for (int i = 0; i < rows * cols; i++)
        vec.push_back(i);

    Matrix<int> matrix(vec, rows, cols);

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(matrix.elementAt(r, c), vec[r * cols + c]);

    EXPECT_EQ(matrix.rows(), rows);
    EXPECT_EQ(matrix.cols(), cols);
}

TEST(ConstructorTests, MatrixCopy_Vector)
{
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS;

    vector<int> vec;

    for (int i = 0; i < rows * cols; i++)
        vec.push_back(i);

    Matrix<int> matrixA(vec, rows, cols);

    Matrix<int> matrixB(matrixA);

    EXPECT_EQ(matrixA.rows(), matrixB.rows());
    EXPECT_EQ(matrixA.cols(), matrixB.cols());
    
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(matrixA.elementAt(r, c), matrixB.elementAt(r, c));
}

TEST(Addition, Addition_SquareMatrix)
{
    const int targetValue = 5;
    const int rows = 10;
    const int cols = 10;

    int* arrA = new int[rows * cols];
    int* arrB = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++) {
        arrA[i] = i;
        arrB[i] = targetValue - i;
    }

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> matrixB(arrB, rows, cols);
    Matrix<int> &additionResult = matrixA + matrixB;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(additionResult.elementAt(r, c), targetValue);
}

TEST(Addition, Addition_RectangularMatrix)
{
    const int targetValue = 5;
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS_HALF;

    int* arrA = new int[rows * cols];
    int* arrB = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++) {
        arrA[i] = i;
        arrB[i] = targetValue - i;
    }

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> matrixB(arrB, rows, cols);
    Matrix<int> &additionResult = matrixA + matrixB;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(additionResult.elementAt(r, c), targetValue);
}

TEST(Subtraction, Subtraction_SquareMatrix)
{
    const int targetValue = 5;
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS;

    int* arrA = new int[rows * cols];
    int* arrB = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++) {
        arrA[i] = 2 * targetValue;
        arrB[i] = targetValue;
    }

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> matrixB(arrB, rows, cols);
    Matrix<int> &subtractionResult = matrixA - matrixB;

    for (int r = 0; r < rows; r++)
    for (int c = 0; c < cols; c++)
            EXPECT_EQ(subtractionResult.elementAt(r, c), targetValue);
}

TEST(Subtraction, Subtraction_RectangularMatrix)
{
    const int targetValue = 5;
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS_HALF;

    int* arrA = new int[rows * cols];
    int* arrB = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++) {
        arrA[i] = 2 * targetValue;
        arrB[i] = targetValue;
    }

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> matrixB(arrB, rows, cols);
    Matrix<int> &subtractionResult = matrixA - matrixB;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(subtractionResult.elementAt(r, c), targetValue);
}

TEST(ScalarMultiplication, Multiplication_SquareMatrix)
{
    const int targetValue = 6;
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS;

    int* arrA = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++)
        arrA[i] = targetValue / 2;

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> &scalarMultiplicationResult = matrixA * 2;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(scalarMultiplicationResult.elementAt(r, c), targetValue);
}

TEST(ScalarMultiplication, Multiplication_RectangularMatrix)
{
    const int targetValue = 6;
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS_HALF;

    int* arrA = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++)
        arrA[i] = targetValue / 2;

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> &scalarMultiplicationResult = matrixA * 2;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(scalarMultiplicationResult.elementAt(r, c), targetValue);
}

TEST(MatrixMultiplication, Multiplication_SquareMatrix)
{
    const int rows = 2;
    const int cols = 2;

    int arrA[rows * cols] = {1, 2, 3, 4};
    int arrB[rows * cols] = {1, 2, 3, 4};
    int solutionArray[rows * cols] = {7, 10, 15, 22};

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> matrixB(arrB, rows, cols);
    Matrix<int> matrixMultiplicationActualSolution(solutionArray, rows, cols);

    Matrix<int> &matrixMultiplicationCalculatedSolution = matrixA * matrixB;

    EXPECT_TRUE(matrixMultiplicationCalculatedSolution.equals(matrixMultiplicationActualSolution));
}

// TEST(MatrixMultiplication, Multiplication_MismatchMatrixSizes)
// {
//     const int matARows = 3;
//     const int matACols = 2;

//     const int matBRows = 2;
//     const int matBCols = 3;

//     int arrA[matARows * matACols] = {1, 2, 3, 4, 5, 6};
//     int arrB[matBRows * matBCols] = {1, 2, 3, 4, 5, 6};
//     int solutionArray[matARows * matBCols] = {9, 12, 15, 19, 26, 33, 29, 40, 51};

//     Matrix<int> matrixA(arrA, matARows, matACols);
//     Matrix<int> matrixB(arrB, matBRows, matBCols);
//     Matrix<int> matrixMultiplicationActualSolution(solutionArray, matARows, matBCols);

//     Matrix<int> &matrixMultiplicationCalculatedSolution = matrixA * matrixB;

//     EXPECT_TRUE(matrixMultiplicationCalculatedSolution.equals(matrixMultiplicationActualSolution));
// }

// TEST(Determinant, Determinant)
// {
// }

// TEST(MatrixInverse, Inverse)
// {
// }

// TEST(MatrixDivision, Division)
// {
//     const int rows = 2;
//     const int cols = 2;

//     int arrA[rows * cols] = {1, 2, 3, 4};
//     int arrB[rows * cols] = {1, 2, 3, 4};
//     int solutionArray[rows * cols] = {1, 0, 1, 0};

//     Matrix<int> matrixA(arrA, rows, cols);
//     Matrix<int> matrixB(arrB, rows, cols);
//     Matrix<int> matrixDivisionActualSolution(solutionArray, rows, cols);

//     Matrix<int> &matrixDivisionCalculatedSolution = matrixA / matrixB;

//     EXPECT_TRUE(matrixDivisionCalculatedSolution.equals(matrixDivisionActualSolution));
// }

TEST(scalarDivision, Division_SquareMatrix)
{
    const int targetValue = 6;
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS;

    int* arrA = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++)
        arrA[i] = targetValue * 2;

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> &scalarDivisionResult = matrixA / 2;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(scalarDivisionResult.elementAt(r, c), targetValue);
}

TEST(scalarDivision, Division_RectangularMatrix)
{
    const int targetValue = 6;
    const int rows = MATRIX_NUM_ROWS;
    const int cols = MATRIX_NUM_COLS_HALF;

    int* arrA = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++)
        arrA[i] = targetValue * 2;

    Matrix<int> matrixA(arrA, rows, cols);
    Matrix<int> &scalarMultiplicationResult = matrixA / 2;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            EXPECT_EQ(scalarMultiplicationResult.elementAt(r, c), targetValue);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
